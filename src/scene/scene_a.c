#include "scene_a.h"

#include "scene.h"
#include "t3d/t3dmodel.h"

scene_t scene_a = (scene_t) {
	.name = "scene a",

	.clear_color = (color_t) { 0xFF, 0xFF, 0xFF, 0xFF },
	.ambient_color = (uint8_t[]) { 200, 200, 200, 0xFF },

	.fog_enabled = true,
	.fog_color = (color_t) { 0xFF, 0xFF, 0xFF, 0xFF },
	.fog_near = 50.0f,
	.fog_far = 500.0f,

	.load = &scene_a_load,
	.update = &scene_a_update,
	.render = NULL,
};

actor_t* the_cube;

void scene_a_load(scene_t* this) {
	the_cube = malloc(sizeof(actor_t));
	*the_cube = (actor_t) {
		.transform = {
			.position = { { 0, 0, 0 } },
			.rotation = { { 0, 0, 0 } },
			.scale = { { 1, 1, 1 } },
		},
		.transform_matrix = malloc_uncached(sizeof(T3DMat4FP)),
		.model = t3d_model_load("rom://models/test_cube.t3dm"),
	};

	scene_add_actor(&scene_a, the_cube);
}

void scene_a_update(scene_t* this, float delta_time, float elapsed) {
	the_cube->transform.position.x = fm_cosf(elapsed * M_TWOPI) * 100.0f;
	the_cube->transform.position.y = fm_sinf(elapsed * M_TWOPI) * 100.0f;
}
