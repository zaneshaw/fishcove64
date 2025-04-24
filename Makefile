# https://github.com/SpookyIluha/DDLC64-LibdragonVNE/blob/engine-game-example/Makefile
# https://github.com/alexvnesta/n64_bullet_demo/blob/main/Makefile

BUILD_DIR = build
SOURCE_DIR = src

PRETTY_NAME = "Fish Cove 64"
PROJECT_NAME = fishcove64

include $(N64_INST)/include/n64.mk
include $(N64_INST)/include/t3d.mk

INCLUDE += -I$(N64_INST)/include -Iinclude -Isrc # needed?
CFLAGS += -I$(N64_INST)/include -I$(N64_INST)/include/ccd -std=gnu17
LDFLAGS += -g -L$(N64_INST)/lib -l:libccd.a

SOURCES = $(shell find $(SOURCE_DIR) -type f -name '*.c')

FONT_LIST = $(shell find assets/fonts/ -type f -name '*.ttf')
IMAGE_LIST = $(shell find assets/images/ -type f -name '*.png')
TEXTURE_LIST = $(shell find assets/models/textures/ -type f -name '*.png')
MODEL_LIST = $(shell find assets/models/ -type f -name '*.glb')
SOUND_LIST = $(shell find assets/sound/ -type f -name '*.wav')

ASSETS_LIST += $(subst assets,filesystem,$(FONT_LIST:%.ttf=%.font64))
ASSETS_LIST += $(subst assets,filesystem,$(IMAGE_LIST:%.png=%.sprite))
ASSETS_LIST += $(subst assets,filesystem,$(TEXTURE_LIST:%.png=%.sprite))
ASSETS_LIST += $(subst assets,filesystem,$(MODEL_LIST:%.glb=%.t3dm))
ASSETS_LIST += $(subst assets,filesystem,$(SOUND_LIST:%.wav=%.wav64))

filesystem/images/%.sprite: assets/images/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	$(N64_MKSPRITE) $(MKSPRITE_FLAGS) --compress 1 --dither ORDERED -o $(dir $@) "$<"

filesystem/models/textures/%.sprite: assets/models/textures/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	$(N64_MKSPRITE) $(MKSPRITE_FLAGS) --compress 1 --dither ORDERED -o $(dir $@) "$<"

filesystem/sound/%.wav64: assets/sound/%.wav
	@mkdir -p $(dir $@)
	@echo "    [SOUND] $@"
	$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) --wav-compress 1 -o $(dir $@) "$<"

filesystem/fonts/%.font64: assets/fonts/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	$(N64_MKFONT) $(MKFONT_FLAGS) --outline 1 -o $(dir $@) "$<"

filesystem/models/%.t3dm: assets/models/%.glb
	@mkdir -p $(dir $@)
	@echo "    [MODEL] $@"
	$(T3D_GLTF_TO_3D) "$<" $@ --base-scale=256

all: $(PROJECT_NAME).z64

$(BUILD_DIR)/$(PROJECT_NAME).dfs: $(ASSETS_LIST)
$(BUILD_DIR)/$(PROJECT_NAME).elf: $(SOURCES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

$(PROJECT_NAME).z64: N64_ROM_TITLE=$(PRETTY_NAME)
$(PROJECT_NAME).z64: $(BUILD_DIR)/$(PROJECT_NAME).dfs

clean:
	rm -rf $(BUILD_DIR) *.z64
	rm -rf filesystem

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
