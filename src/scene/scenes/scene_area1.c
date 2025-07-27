#include "../../collision/collision.h"
#include "../../game/fishing.h"
#include "../../player/inventory.h"
#include "../scene.h"

#include <t3d/t3dmodel.h>

static void load(scene_t* this);

scene_t scene_area1 = (scene_t) {
	.name = "Area 1",

	.clear_color = (color_t) { 0xFF, 0xFF, 0xFF, 0xFF },
	.ambient_color = (uint8_t[]) { 0xA0, 0xA0, 0xC8, 0xFF },
	.sun_color = (uint8_t[]) { 0xC8, 0xC8, 0xFF, 0xFF },
	.sun_dir = (fm_vec3_t) { { 0.4f, 1.0f, -1.0f } },

	.fog_enabled = false,

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

		.collision_count = 1,
		.collisions = collision_allocate(
			1,
			(collision_t[]) {
				{
					.type = COLLISION_SHAPE_BOX,
					.flags = COLLISION_FLAG_COLLIDE,
					.shape = &(box_t) {
						.pos = { 0, -100, 0 },
						.half_extents = { 10000, 100, 10000 },
						.quat = { { 0, 0, 0, 1 } },
					},
				},
			}
		),
	};

	scene_add_actor(&scene_area1, world);
}
