#include <t3d/t3dmodel.h>
#include <debug.h>

float delta_time;
float elapsed = 0.0f;

void setup() {
	debug_init_isviewer();

	// rdpq_config_enable(RDPQ_CFG_AUTOSYNCPIPE);
}

void render() {
	// t3d_matrix_push_pos(1);

	// for (int i = 0; i < actor_count; i++) {
	// 	actor_draw(actors[i]);
	// }

	// t3d_matrix_pop(1);
}

int main() {
	display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
	rdpq_init();
	t3d_init((T3DInitParams) {});
	dfs_init(DFS_DEFAULT_LOCATION);
	joypad_init();

	setup();

	T3DViewport viewport = t3d_viewport_create();

	for (uint64_t frame = 0;; ++frame) {
		delta_time = display_get_delta_time();
		elapsed += delta_time;

		t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(70.0f), 15.0f, 350.0f);

		rdpq_attach(display_get(), display_get_zbuf());

		t3d_frame_start();
		t3d_viewport_attach(&viewport);

		rdpq_set_prim_color((color_t) { 0xFF, 0xFF, 0xFF, 0xFF });

		rdpq_mode_fog(RDPQ_FOG_STANDARD);
		rdpq_set_fog_color(RGBA16(0x00, 0x00, 0x00, 0xFF));

		t3d_screen_clear_depth();

		t3d_fog_set_range(50.0f, 500.0f);
		t3d_fog_set_enabled(true);

		render();

		rdpq_detach_show();
	}

	t3d_destroy();
	return 0;
}
