#include "scene.h"

#include "../collision/collision.h"

scene_t* current_scene = NULL;

void scene_add_box_collisions(scene_t* scene, box_t* boxes, int count) {
	scene->collision_boxes = boxes;
	scene->collision_boxes_count = count;
}

void scene_load(scene_t* scene) {
	if (current_scene != NULL) {
		scene_kill(scene);
	}

	// todo: load collisions from a json file
	box_t* boxes = malloc(sizeof(box_t) * 7);

	box_t _boxes[] = {
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
	};

	memcpy(boxes, _boxes, sizeof(box_t) * 7);

	scene_add_box_collisions(scene, boxes, 7);

	current_scene = scene;
	current_scene->load(current_scene);
}

void scene_update(float delta_time, float elapsed) {
	if (current_scene->update) current_scene->update(current_scene, delta_time, elapsed);

	for (int i = 0; i < current_scene->_actor_count; i++) {
		actor_t* actor = current_scene->_actors[i];
		vector3_t scale;
		vector3_scale(&scale, &actor->transform.scale, 100.0f / 256.0f);

		t3d_mat4fp_from_srt_euler(
			actor->transform_matrix,
			vector3_to_fgeom(scale).v,
			vector3_to_fgeom(actor->transform.rotation).v,
			vector3_to_fgeom(actor->transform.position).v
		);
	}
}

void scene_render() {
	t3d_screen_clear_color(current_scene->clear_color);

	t3d_light_set_ambient(current_scene->ambient_color);

	if (current_scene->fog_enabled) {
		rdpq_mode_fog(RDPQ_FOG_STANDARD);
		rdpq_set_fog_color(current_scene->fog_color);

		t3d_fog_set_range(current_scene->fog_near, current_scene->fog_far);
		t3d_fog_set_enabled(true);
	} else {
		t3d_fog_set_enabled(false);
	}

	if (current_scene->render) current_scene->render(current_scene);

	for (int i = 0; i < current_scene->_actor_count; i++) {
		actor_draw(current_scene->_actors[i]);
	}
}

void scene_kill(scene_t* scene) {
	for (int i = 0; i < current_scene->_actor_count; i++) {
		actor_kill(current_scene->_actors[i]);
	}

	scene->_actor_count = 0;
	scene->_actor_capacity = 0;
	free(scene->_actors);

	free(scene->collision_boxes);
	scene->collision_boxes = 0x00;
	scene->collision_boxes_count = 0;
}

void scene_add_actor(scene_t* scene, actor_t* actor) {
	if (scene->_actor_count + 1 > scene->_actor_capacity) {
		if (scene->_actor_capacity == 0) {
			scene->_actor_capacity = 4;

			scene->_actors = malloc(sizeof(actor_t*) * 4);
		} else {
			scene->_actor_capacity *= 2;

			actor_t** bigger_actors = malloc(sizeof(actor_t*) * scene->_actor_capacity);

			for (int i = 0; i < scene->_actor_count; i++) {
				bigger_actors[i] = scene->_actors[i];
			}

			free(scene->_actors);

			scene->_actors = bigger_actors;
		}
	}

	scene->_actors[scene->_actor_count] = actor;
	scene->_actor_count++;
}
