#include "scene_loader.h"

#include "../collision/collision.h"
#include "../collision/shapes/box.h"
#include "../collision/shapes/cylinder.h"
#include "../util/constants.h"

scene_t* current_scene = NULL;

void scene_load(scene_t* scene) {
	if (current_scene != NULL) {
		scene_kill(scene);
	}

	current_scene = scene;
	current_scene->load(current_scene);
}
