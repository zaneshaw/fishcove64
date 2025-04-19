#include "scene.h"

#include <stddef.h>

scene_t* current_scene = NULL;

void scene_load(scene_t* scene) {
	if (current_scene != NULL) {
		scene_kill(scene);
	}

	current_scene = scene;
	current_scene->load(current_scene);
}

void scene_update(float delta_time, float elapsed) {
	if (current_scene->update) current_scene->update(current_scene, delta_time, elapsed);

	for (int i = 0; i < current_scene->_actor_count; i++) {
		actor_t* actor = current_scene->_actors[i];

		t3d_mat4fp_from_srt_euler(
			actor->transform_matrix,
			(fm_vec3_t) { {
				actor->transform.scale.x * (100.0f / 256.0f),
				actor->transform.scale.y * (100.0f / 256.0f),
				actor->transform.scale.z * (100.0f / 256.0f),
			} }.v,
			actor->transform.rotation.v,
			actor->transform.position.v
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
