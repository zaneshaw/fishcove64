#include "../scene.h"

#include <t3d/t3dmodel.h>

static void load(scene_t* this);

scene_t scene_collision_test = (scene_t) {
	.name = "Collision Testing Zone",

	.clear_color = (color_t) { 0x00, 0x00, 0x00, 0xFF },
	.ambient_color = (uint8_t[]) { 0x7F, 0x7F, 0x7F, 0xFF },
	.sun_color = (uint8_t[]) { 0xFF, 0xFF, 0xFF, 0xFF },
	.sun_dir = (fm_vec3_t) { { 0.4f, 0.4f, -1.0f } },

	.fog_enabled = false,

	.collision = {
		.path = "/files/collision_test.col",
	},

	.spawn_position = (vector3_t) { 0, 0, -300 },

	.load = &load,
	.update = NULL,
	.render = NULL,
};

static actor_t* world;

static void load(scene_t* this) {
	world = malloc(sizeof(actor_t));
	*world = (actor_t) {
		.transform = {
			.position = { 0, 0, 0 },
			.rotation = { 0, 0, 0 },
			.scale = { 1, 1, 1 },
		},
		.transform_matrix = malloc_uncached(sizeof(T3DMat4FP)),
		.model = t3d_model_load("rom:/models/scenes/collision_test/world.t3dm"),
	};

	scene_add_actor(&scene_collision_test, world);
}
