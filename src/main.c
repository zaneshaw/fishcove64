#include "actor/actor.h"
#include "collision/collision.h"
#include "collision/shapes/box.h"
#include "collision/shapes/capsule.h"
#include "debug/debug_draw.h"
#include "debug/debug_menu.h"
#include "font/font.h"
#include "game/game.h"
#include "lib/pcg/pcg_basic.h"
#include "lib/debug.h"
#include "math/vector3.h"
#include "player/inventory.h"
#include "player/player.h"
#include "save/save.h"
#include "scene/scene_loader.h"
#include "scene/scene_playground.h"
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

	scene_load(&scene_playground);

	debug_printcommands();
}

int main() {
	display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
	rdpq_init();
	t3d_init((T3DInitParams) {});
	gl_init();
	dfs_init(DFS_DEFAULT_LOCATION);
	joypad_init();
	rtc_init();

	// display_set_fps_limit(30);

	setup();

	T3DViewport viewport = t3d_viewport_create();

	for (uint64_t frame = 0;; frame++) {
		delta_time = display_get_delta_time();
		elapsed += delta_time;
		now = time(NULL);

		scene_update(delta_time, elapsed);

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

		rdpq_attach(display_get(), display_get_zbuf());

		debug_draw_look();

		t3d_frame_start();
		t3d_viewport_attach(&viewport);

		rdpq_set_prim_color(RGBA16(0xFF, 0xFF, 0xFF, 0xFF));

		t3d_screen_clear_depth();

		scene_render();

		if (BOX_DEBUG_LEVEL != BOX_DEBUG_NONE) {
			for (int i = 0; i < current_scene->collisions_count; i++) {
				if (current_scene->collisions[i].type == COLLISION_SHAPE_BOX) {
					box_t* box = current_scene->collisions[i].shape;

					// idc. it works
					float red = (BOX_DEBUG_LEVEL & BOX_DEBUG_COLLIDE) == BOX_DEBUG_COLLIDE && (box->flags & COLLISION_FLAG_COLLIDE) == COLLISION_FLAG_COLLIDE ? 1.0f : 0.0f;
					float blue = (BOX_DEBUG_LEVEL & BOX_DEBUG_INTERACT) == BOX_DEBUG_INTERACT && (box->flags & COLLISION_FLAG_INTERACT) == COLLISION_FLAG_INTERACT ? 1.0f : 0.0f;

					if (red > 0.0f || blue > 0.0f) {
						debug_draw_box(box, &(fm_vec3_t) { { red, 0.0f, blue } });
					}
				}
			}
		}

		player_render();
		if (game_input_state == GAME_INPUT_PAUSE) pause_render();
		debug_menu_render(delta_time, elapsed, now);

		rdpq_detach_show();
	}

	t3d_destroy();
	return 0;
}
