#include "../../game/fishing.h"
#include "../../player/inventory.h"
#include "../../player/player.h"
#include "../scene.h"

#include <t3d/t3dmodel.h>

static void load(scene_t* this);

scene_t scene_area1 = (scene_t) {
	.name = "Area 1",

	.clear_color = (color_t) { 109, 209, 252, 0xFF },
	.ambient_color = (uint8_t[]) { 160, 160, 200, 0xFF },
	.sun_color = (uint8_t[]) { 0xC8, 0xC8, 0xFF, 0xFF },
	.sun_dir = (fm_vec3_t) { { 0.4f, 0.4f, -1.0f } },

	.fog_enabled = true,
	.fog_color = (color_t) { 109, 209, 252, 0xFF },
	.fog_near = 600.0f,
	.fog_far = 1400.0f,

	.collision = {
		.path = "/files/area1.col",
	},

	.spawn_position = (vector3_t) { -30, 0, -160 },
	.spawn_yaw = -0.85f,

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
		.model = t3d_model_load("rom:/models/scenes/area1/world.t3dm"),
	};

	scene_add_actor(&scene_area1, world);
}
