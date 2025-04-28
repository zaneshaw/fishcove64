#include "actor/actor.h"
#include "collision/collision.h"
#include "collision/shapes/box.h"
#include "collision/shapes/capsule.h"
#include "debug/debug_menu.h"
#include "font/font.h"
#include "math/vector3.h"
#include "player/player.h"
#include "scene/scene_loader.h"
#include "scene/scene_playground.h"
#include "util/macros.h"

#include <debug.h>
#include <joypad.h>
#include <t3d/t3dmodel.h>

float delta_time;
float elapsed = 0.0f;

capsule_t capsule;
box_t box;

void setup() {
	debug_init_isviewer();
	debug_init_usblog();

	// rdpq_config_enable(RDPQ_CFG_AUTOSYNCPIPE);

	font_init();
	debug_menu_init();
	collision_init();

	scene_load(&scene_playground);
}

int main() {
	display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
	rdpq_init();
	t3d_init((T3DInitParams) {});
	dfs_init(DFS_DEFAULT_LOCATION);
	joypad_init();

	setup();

	T3DViewport viewport = t3d_viewport_create();

	for (uint64_t frame = 0;; frame++) {
		delta_time = display_get_delta_time();
		elapsed += delta_time;

		scene_update(delta_time, elapsed);

		joypad_poll();

		player_look(delta_time);
		player_move(delta_time);

		// todo: move to eye/camera struct
		transform_t eye = player_get_eye();
		vector3_t eye_forward = {
			fm_cosf(eye.rotation.x) * fm_sinf(eye.rotation.y),
			-fm_sinf(eye.rotation.x),
			fm_cosf(eye.rotation.x) * fm_cosf(eye.rotation.y),
		};
		vector3_t eye_target;
		vector3_add(&eye_target, &eye.position, &eye_forward);

		t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(70.0f), 20.0f, 1000.0f);
		t3d_viewport_look_at(
			&viewport,
			&vector3_to_fgeom(eye.position),
			&vector3_to_fgeom(eye_target),
			&vector3_to_fgeom(vector3_up)
		);

		rdpq_attach(display_get(), display_get_zbuf());

		t3d_frame_start();
		t3d_viewport_attach(&viewport);

		rdpq_set_prim_color(RGBA16(0xFF, 0xFF, 0xFF, 0xFF));

		t3d_screen_clear_depth();

		if (JOYPAD_IS_READY) {
			if (joypad_get_buttons_pressed(JOYPAD).l) {
				debug_menu_toggle();
			}
		}

		scene_render();

		debug_menu_render(delta_time, elapsed);

		rdpq_detach_show();
	}

	t3d_destroy();
	return 0;
}
