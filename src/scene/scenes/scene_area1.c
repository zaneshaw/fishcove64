#include "../../collision/collision.h"
#include "../../game/fishing.h"
#include "../../player/inventory.h"
#include "../scene.h"

#include <t3d/t3dmodel.h>

static void load(scene_t* this);

scene_t scene_area1 = (scene_t) {
	.name = "Area 1",

	.clear_color = (color_t) { 0xFF, 0xFF, 0xFF, 0xFF },
	.ambient_color = (uint8_t[]) { 200, 200, 200, 0xFF },

	.fog_enabled = true,
	.fog_color = (color_t) { 0xFF, 0xFF, 0xFF, 0xFF },
	.fog_near = 200.0f,
	.fog_far = 400.0f,

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
						.half_extents = { 1000, 100, 1000 },
						.quat = { { 0, 0, 0, 1 } },
					},
				},
			}
		),
	};

	scene_add_actor(&scene_area1, world);
}
