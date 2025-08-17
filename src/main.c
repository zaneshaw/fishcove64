#include "actor/actor.h"
#include "collision/collision.h"
#include "collision/shapes/box.h"
#include "collision/shapes/capsule.h"
#include "debug/debug_draw.h"
#include "debug/debug_menu.h"
#include "font/font.h"
#include "game/game.h"
#include "lib/debug.h"
#include "lib/pcg/pcg_basic.h"
#include "math/vector3.h"
#include "player/inventory.h"
#include "player/player.h"
#include "save/save.h"
#include "scene/scene.h"
#include "scene/scene_loader.h"
#include "util/constants.h"

#include <GL/gl_integration.h>
#include <debug.h>
#include <joypad.h>
#include <t3d/t3dmodel.h>
#include <time.h>

float delta_time;
float elapsed = 0.0f;
time_t now;
save_data_t last_data;

void setup() {
	debug_init_isviewer();
	debug_init_usblog();

	debug_initialize();

	pcg32_srandom(0, getentropy32());

	font_init();
	debug_menu_init();
	collision_init();
	fishing_init();
	debug_draw_init();
	player_init();

	scene_load(&scene_area1);

	debug_printcommands();
}

int main() {
	display_init((resolution_t) { .width = 320L, .height = 288L }, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
	rdpq_init();
	t3d_init((T3DInitParams) {});
	gl_init();
	dfs_init(DFS_DEFAULT_LOCATION);
	joypad_init();
	rtc_init();

	// display_set_fps_limit(30);

	setup();

	T3DViewport viewport = t3d_viewport_create();
	t3d_viewport_set_area(
		&viewport,
		OVERSCAN_PAD_X,
		OVERSCAN_PAD_Y,
		(int32_t) display_get_width() - OVERSCAN_PAD_X * 2,
		(int32_t) display_get_height() - OVERSCAN_PAD_Y * 2
	);

	for (uint64_t frame = 0;; frame++) {
		delta_time = display_get_delta_time();
		elapsed += delta_time;
		now = time(NULL);

		joypad_poll();

		if (JOYPAD_IS_READY) {
			joypad_buttons_t buttons = joypad_get_buttons(JOYPAD);
			joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD);
			if (buttons.z) {
				if (pressed.l) debug_menu_toggle();
				if (pressed.r) debug_time_toggle();
			}

			if (game_input_state == GAME_INPUT_NORMAL) {
				if (pressed.d_right) {
					inventory_toggle();
				}
			} else if (game_input_state == GAME_INPUT_PAUSE) {
				if (pressed.a) {
					load(&last_data);
					elapsed = last_data.elapsed;
				}
				if (pressed.b) save(elapsed);

				if (pressed.d_left) {
					scene_load(&scene_playground);
				} else if (pressed.d_right) {
					scene_load(&scene_area1);
				}
			}

			if (pressed.start) {
				game_input_state = (game_input_state + 1) % 2;
			}
		}

		player_look(delta_time);
		player_move(delta_time);
		player_interact();

		// todo: move to eye/camera struct
		transform_t eye = player_get_eye();
		vector3_t eye_forward = {
			fm_cosf(eye.rotation.x) * fm_sinf(eye.rotation.y),
			-fm_sinf(eye.rotation.x),
			fm_cosf(eye.rotation.x) * fm_cosf(eye.rotation.y),
		};
		vector3_t eye_target;
		vector3_add(&eye_target, &eye.position, &eye_forward);

		t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(FOV), NEAR_PLANE, FAR_PLANE);
		t3d_viewport_look_at(
			&viewport,
			&vector3_to_fgeom(eye.position),
			&vector3_to_fgeom(eye_target),
			&vector3_to_fgeom(vector3_up)
		);

		scene_update(delta_time, elapsed);

		rdpq_attach(display_get(), display_get_zbuf());

		debug_draw_look();

		t3d_frame_start();
		t3d_viewport_attach(&viewport);

		rdpq_set_prim_color(RGBA16(0xFF, 0xFF, 0xFF, 0xFF));

		t3d_screen_clear_depth();

		scene_render();
		player_render();

		if (game_input_state == GAME_INPUT_PAUSE) pause_render();
		debug_menu_render(delta_time, elapsed, now);

		rdpq_detach_show();
	}

	t3d_destroy();
	return 0;
}
