#include "actor/actor.h"
#include "debug/debug_menu.h"
#include "scene/scene.h"
#include "scene/scene_playground.h"

#include <debug.h>
#include <joypad.h>
#include <t3d/t3dmodel.h>

float delta_time;
float elapsed = 0.0f;

void setup() {
	debug_init_isviewer();

	// rdpq_config_enable(RDPQ_CFG_AUTOSYNCPIPE);

	// todo: move to font.c
	rdpq_font_t* font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO);
	rdpq_text_register_font(1, font);

	scene_load(&scene_playground);
}

int main() {
	display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
	rdpq_init();
	t3d_init((T3DInitParams) {});
	dfs_init(DFS_DEFAULT_LOCATION);
	joypad_init();

	setup();

	T3DViewport viewport = t3d_viewport_create();

	for (uint64_t frame = 0;; frame++) {
		delta_time = display_get_delta_time();
		elapsed += delta_time;

		t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(70.0f), 35.0f, 1000.0f);
		t3d_viewport_look_at(
			&viewport,
			&(fm_vec3_t) { { 0, 180, -500 } },
			&(fm_vec3_t) { { 0, 180, 0 } },
			&(fm_vec3_t) { { 0, 1, 0 } }
		);

		rdpq_attach(display_get(), display_get_zbuf());

		t3d_frame_start();
		t3d_viewport_attach(&viewport);

		rdpq_set_prim_color(RGBA16(0xFF, 0xFF, 0xFF, 0xFF));

		t3d_screen_clear_depth();

		joypad_poll();

		// todo: move to controls.c
		if (joypad_is_connected(JOYPAD_PORT_1) && joypad_get_identifier(JOYPAD_PORT_1) == JOYBUS_IDENTIFIER_N64_CONTROLLER) {
			if (joypad_get_buttons_pressed(JOYPAD_PORT_1).c_up) {
				debug_menu_toggle();
			}
		}

		scene_update(delta_time, elapsed);
		scene_render();

		debug_menu_render(delta_time, elapsed);

		rdpq_detach_show();
	}

	t3d_destroy();
	return 0;
}
