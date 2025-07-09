#include "scene_playground.h"

#include "../collision/collision.h"
#include "scene.h"

#include <t3d/t3dmodel.h>

scene_t scene_playground = (scene_t) {
	.name = "Playground",

	.clear_color = (color_t) { 0xFF, 0xFF, 0xFF, 0xFF },
	.ambient_color = (uint8_t[]) { 200, 200, 200, 0xFF },

	.fog_enabled = true,
	.fog_color = (color_t) { 0xFF, 0xFF, 0xFF, 0xFF },
	.fog_near = 200.0f,
	.fog_far = 400.0f,

	.load = &scene_playground_load,
	.update = &scene_playground_update,
	.render = NULL,
};

actor_t* world;
actor_t* coolest_totem;

void scene_playground_load(scene_t* this) {
	world = malloc(sizeof(actor_t));
	*world = (actor_t) {
		.transform = {
			.position = { 0, 1, 0 },
			.rotation = { 0, 0, 0 },
			.scale = { 1, 1, 1 },
		},
		.transform_matrix = malloc_uncached(sizeof(T3DMat4FP)),
		.model = t3d_model_load("rom:/models/scene_playground.t3dm"),
		.collision_count = 8,
		.collisions = collision_allocate(
			8,
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
				{
					.type = COLLISION_SHAPE_BOX,
					.flags = COLLISION_FLAG_COLLIDE,
					.shape = &(box_t) {
						.pos = { 300, 200, 300 },
						.half_extents = { 50, 200, 50 },
						.quat = { { 0, 0, 0, 1 } },
					},
				},
				{
					.type = COLLISION_SHAPE_BOX,
					.flags = COLLISION_FLAG_COLLIDE,
					.shape = &(box_t) {
						.pos = { -300, 200, 300 },
						.half_extents = { 50, 200, 50 },
						.quat = { { 0, 0, 0, 1 } },
					},
				},
				{
					.type = COLLISION_SHAPE_BOX,
					.flags = COLLISION_FLAG_COLLIDE,
					.shape = &(box_t) {
						.pos = { 300, 200, -300 },
						.half_extents = { 50, 200, 50 },
						.quat = { { 0, 0, 0, 1 } },
					},
				},
				{
					.type = COLLISION_SHAPE_BOX,
					.flags = COLLISION_FLAG_COLLIDE,
					.shape = &(box_t) {
						.pos = { -300, 200, -300 },
						.half_extents = { 50, 200, 50 },
						.quat = { { 0, 0, 0, 1 } },
					},
				},
				{
					.type = COLLISION_SHAPE_BOX,
					.flags = COLLISION_FLAG_INTERACT,
					.shape = &(box_t) {
						.pos = { -200, 150, 200 },
						.half_extents = { 50, 25, 25 },
						.quat = { { 0.3061862, 0.1767767, -0.3061862, 0.8838835 } },
					},
				},
				{
					.type = COLLISION_SHAPE_BOX,
					.flags = COLLISION_FLAG_COLLIDE | COLLISION_FLAG_INTERACT,
					.shape = &(box_t) {
						.pos = { 200, 0, 200 },
						.half_extents = { 50, 25, 25 },
						.quat = { { 0.3061862, 0.1767767, -0.3061862, 0.8838835 } },
					},
				},
				{
					.type = COLLISION_SHAPE_CYLINDER,
					.flags = COLLISION_FLAG_INTERACT,
					.shape = &(cylinder_t) {
						.a = { { 0, 0, 300 } },
						.b = { { 0, 100, 300 } },
						.radius = 100,
					},
				},
			}
		),
		.interaction = {
			.enabled = true,
			.label = "i am the world",
		}
	};

	coolest_totem = malloc(sizeof(actor_t));
	*coolest_totem = (actor_t) {
		.transform = {
			.position = { 0, 1, 0 },
			.rotation = { 0, 0, 0 },
			.scale = { 1, 1, 1 },
		},
		.transform_matrix = malloc_uncached(sizeof(T3DMat4FP)),
		.model = t3d_model_load("rom:/models/coolest_totem.t3dm"),
	};

	scene_add_actor(&scene_playground, world);
	scene_add_actor(&scene_playground, coolest_totem);
}

void scene_playground_update(scene_t* this, float delta_time, float elapsed) {
	coolest_totem->transform.position.y = fm_sinf(elapsed * M_TWOPI * 0.3f) * 100.0f + 180.0f;
}
