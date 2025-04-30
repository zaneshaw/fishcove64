#include "inventory.h"

#include "../util/constants.h"

#include <debug.h>
#include <libdragon.h>

slot_t slots[INVENTORY_SLOTS];

void inventory_add(fish_instance_t* fish_instance) {
	for (int i = 0; i < INVENTORY_SLOTS; i++) {
		if (!slots[i].not_empty) {
			slots[i].not_empty = true;
			slots[i].fish_instance = *fish_instance;
			break;
		}
	}
}

void inventory_render() {
	int x = display_get_width() - OVERSCAN_PAD_X - 32 * 3;
	int y = display_get_height() - OVERSCAN_PAD_Y - 32 * 4;

	rdpq_sync_pipe();
	rdpq_mode_push();

	rdpq_set_mode_standard();
	rdpq_set_prim_color(RGBA32(0x00, 0x00, 0x00, 150));
	rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_fill_rectangle(x, y, x + 32 * 3, y + 32 * 4);

	rdpq_set_mode_copy(true);
	for (int i = 0; i < INVENTORY_SLOTS; i++) {
		if (slots[i].not_empty) {
			rdpq_sprite_blit(
				slots[i].fish_instance.fish->sprite,
				x + ((i % 3) * 32),
				y + ((i / 3) * 32),
				NULL
			);
		}
	}

	rdpq_mode_pop();
}

void inventory_debug() {
	for (int i = 0; i < INVENTORY_SLOTS; i++) {
		if (slots[i].not_empty) {
			debugf("%s (%.2fm long)\n", slots[i].fish_instance.fish->name, slots[i].fish_instance.length);
		}
	}
}
