#include "../../game/fishing.h"
#include "../../player/inventory.h"
#include "../scene.h"

#include <t3d/t3dmodel.h>

static void load(scene_t* this);
static void update(scene_t* this, float delta_time, float elapsed);

scene_t scene_playground = (scene_t) {
	.name = "Playground",

	.clear_color = (color_t) { 0xFF, 0xFF, 0xFF, 0xFF },
	.ambient_color = (uint8_t[]) { 200, 200, 200, 0xFF },
	.sun_color = (uint8_t[]) { 0xC8, 0xC8, 0xFF, 0xFF },
	.sun_dir = (fm_vec3_t) { { 0.4f, 1.0f, -1.0f } },

	.fog_enabled = true,
	.fog_color = (color_t) { 0xFF, 0xFF, 0xFF, 0xFF },
	.fog_near = 200.0f,
	.fog_far = 400.0f,

	.spawn_position = (vector3_t) { 0, 0, -300 },

	.load = &load,
	.update = &update,
	.render = NULL,
};

static actor_t* world;
static actor_t* coolest_totem;
static actor_t* pond;

static void pond_interact() {
	fish_instance_t fish_instance;
	fishing_roll(&fish_instance);
	fishing_debug(false);
	inventory_add(&fish_instance);
}

static void load(scene_t* this) {
	world = malloc(sizeof(actor_t));
	*world = (actor_t) {
		.transform = {
			.position = { 0, 0, 0 },
			.rotation = { 0, 0, 0 },
			.scale = { 1, 1, 1 },
		},
		.transform_matrix = malloc_uncached(sizeof(T3DMat4FP)),
		.model = t3d_model_load("rom:/models/scenes/playground/world.t3dm"),

		.label = "i am the world",
	};

	coolest_totem = malloc(sizeof(actor_t));
	*coolest_totem = (actor_t) {
		.transform = {
			.position = { 0, 180, 0 },
			.rotation = { 0, 0, 0 },
			.scale = { 1, 1, 1 },
		},
		.transform_matrix = malloc_uncached(sizeof(T3DMat4FP)),
		.model = t3d_model_load("rom:/models/scenes/playground/coolest_totem.t3dm"),
	};

	pond = malloc(sizeof(actor_t));
	*pond = (actor_t) {
		.transform = {
			.position = { 0, 0, 300 },
			.rotation = { 0, 0, 0 },
			.scale = { 1, 1, 1 },
		},
		.transform_matrix = malloc_uncached(sizeof(T3DMat4FP)),
		.model = t3d_model_load("rom:/models/scenes/playground/pond.t3dm"),

		.label = "i am the water pond",
		.interact = &pond_interact,
	};

	scene_add_actor(&scene_playground, world);
	scene_add_actor(&scene_playground, coolest_totem);
	scene_add_actor(&scene_playground, pond);
}

static void update(scene_t* this, float delta_time, float elapsed) {
	coolest_totem->transform.position.y = fm_sinf(elapsed * M_TWOPI * 0.3f) * 100.0f + 180.0f;
}
