#include "scene_loader.h"

#include "../collision/collision.h"
#include "../collision/shapes/box.h"
#include "../collision/shapes/cylinder.h"
#include "../lib/cjson/cJSON.h"
#include "../util/macros.h"

scene_t* current_scene = NULL;

box_t* load_box(cJSON* attr) {
	box_t* box = malloc(sizeof(box_t));

	while (attr) {
		if (str_eq(attr->string, "px")) box->pos.x = attr->valuedouble;
		else if (str_eq(attr->string, "py")) box->pos.y = attr->valuedouble;
		else if (str_eq(attr->string, "pz")) box->pos.z = attr->valuedouble;
		else if (str_eq(attr->string, "hex")) box->half_extents.x = attr->valuedouble;
		else if (str_eq(attr->string, "hey")) box->half_extents.y = attr->valuedouble;
		else if (str_eq(attr->string, "hez")) box->half_extents.z = attr->valuedouble;
		else if (str_eq(attr->string, "qx")) box->quat.x = attr->valuedouble;
		else if (str_eq(attr->string, "qy")) box->quat.y = attr->valuedouble;
		else if (str_eq(attr->string, "qz")) box->quat.z = attr->valuedouble;
		else if (str_eq(attr->string, "qw")) box->quat.w = attr->valuedouble;
		else if (str_eq(attr->string, "mode")) box->mode = attr->valueint;

		attr = attr->next;
	}

	return box;
}

void scene_load_collisions(scene_t* scene) {
	int json_size;
	char* file = asset_load(scene->collision_path, &json_size);
	cJSON* json = cJSON_ParseWithLength(file, json_size);

	box_t* boxes[128];
	int boxes_count = 0;

	int i = 0;
	cJSON* col = json->child;
	while (col) {
		cJSON* attr = col->child;
		assertf(str_eq(attr->string, "shape"), "First key of collision %d must be \"shape\"", i);

		if (str_eq(attr->valuestring, "box")) {
			boxes[boxes_count] = load_box(attr);
			boxes_count++;
		}

		i++;
		col = col->next;
	};

	scene->collision_boxes = malloc(sizeof(box_t*) * boxes_count);
	memcpy(scene->collision_boxes, boxes, sizeof(box_t*) * boxes_count);
	scene->collision_boxes_count = boxes_count;

	free(file);
	cJSON_Delete(json);
}

void scene_load(scene_t* scene) {
	if (current_scene != NULL) {
		scene_kill(scene);
	}

	scene_load_collisions(scene);

	current_scene = scene;
	current_scene->load(current_scene);
}
