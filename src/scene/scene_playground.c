#include "scene_playground.h"

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

	.boxes = {
		{
			.pos = { 0.0f, -100.0f, 0.0f },
			.half_extents = { 1000.0f, 100.0f, 1000.0f },
			.quat = { { 0.0f, 0.0f, 0.0f, 1.0f } },
		},
		{
			.pos = { 300.0f, 0.0f, 300.0f },
			.half_extents = { 50.0f, 500.0f, 50.0f },
			.quat = { { 0.0f, 0.0f, 0.0f, 1.0f } },
		},
		{
			.pos = { -300.0f, 0.0f, 300.0f },
			.half_extents = { 50.0f, 500.0f, 50.0f },
			.quat = { { 0.0f, 0.0f, 0.0f, 1.0f } },
		},
		{
			.pos = { 300.0f, 0.0f, -300.0f },
			.half_extents = { 50.0f, 500.0f, 50.0f },
			.quat = { { 0.0f, 0.0f, 0.0f, 1.0f } },
		},
		{
			.pos = { -300.0f, 0.0f, -300.0f },
			.half_extents = { 50.0f, 500.0f, 50.0f },
			.quat = { { 0.0f, 0.0f, 0.0f, 1.0f } },
		},
		{
			.pos = { -200.0f, 150.0f, 200.0f },
			.half_extents = { 50.0f, 25.0f, 25.0f },
			// .quat = { { 0.0f, 0.0f, 0.0f, 1.0f } },
			.quat = { { 0.176777f, 0.306186f, 0.176777f, 0.918559f } },
		},
		{
			.pos = { 200.0f, 0.0f, 200.0f },
			.half_extents = { 50.0f, 25.0f, 25.0f },
			// .quat = { { 0.0f, 0.0f, 0.0f, 1.0f } },
			.quat = { { 0.176777f, 0.306186f, 0.176777f, 0.918559f } },
		},
	}
};

actor_t* scene_model;
actor_t* coolest_totem;

void scene_playground_load(scene_t* this) {
	scene_model = malloc(sizeof(actor_t));
	*scene_model = (actor_t) {
		.transform = {
			.position = { 0, 0, 0 },
			.rotation = { 0, 0, 0 },
			.scale = { 1, 1, 1 },
		},
		.transform_matrix = malloc_uncached(sizeof(T3DMat4FP)),
		.model = t3d_model_load("rom://models/scene_playground.t3dm"),
	};

	coolest_totem = malloc(sizeof(actor_t));
	*coolest_totem = (actor_t) {
		.transform = {
			.position = { 0, 0, 0 },
			.rotation = { 0, 0, 0 },
			.scale = { 1, 1, 1 },
		},
		.transform_matrix = malloc_uncached(sizeof(T3DMat4FP)),
		.model = t3d_model_load("rom://models/coolest_totem.t3dm"),
	};

	scene_add_actor(&scene_playground, scene_model);
	scene_add_actor(&scene_playground, coolest_totem);
}

void scene_playground_update(scene_t* this, float delta_time, float elapsed) {
	// coolest_totem->transform.position.x = fm_cosf(elapsed * M_TWOPI * 0.3f) * 100.0f;
	coolest_totem->transform.position.y = fm_sinf(elapsed * M_TWOPI * 0.3f) * 100.0f + 180.0f;

	// coolest_totem->transform.rotation.y = elapsed * M_PI_2;
}
