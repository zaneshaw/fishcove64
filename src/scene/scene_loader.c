#include "scene_loader.h"

#include "../collision/collision.h"
#include "../collision/shapes/box.h"
#include "../collision/shapes/cylinder.h"
#include "../lib/cjson/cJSON.h"
#include "../util/constants.h"

scene_t* current_scene = NULL;

box_t* load_box(cJSON* attr) {
	box_t* box = malloc(sizeof(box_t));

	while (attr) {
		if (STR_EQ(attr->string, "id")) strcpy(box->id, attr->valuestring);
		else if (STR_EQ(attr->string, "px")) box->pos.x = attr->valuedouble;
		else if (STR_EQ(attr->string, "py")) box->pos.y = attr->valuedouble;
		else if (STR_EQ(attr->string, "pz")) box->pos.z = attr->valuedouble;
		else if (STR_EQ(attr->string, "hex")) box->half_extents.x = attr->valuedouble;
		else if (STR_EQ(attr->string, "hey")) box->half_extents.y = attr->valuedouble;
		else if (STR_EQ(attr->string, "hez")) box->half_extents.z = attr->valuedouble;
		else if (STR_EQ(attr->string, "qx")) box->quat.x = attr->valuedouble;
		else if (STR_EQ(attr->string, "qy")) box->quat.y = attr->valuedouble;
		else if (STR_EQ(attr->string, "qz")) box->quat.z = attr->valuedouble;
		else if (STR_EQ(attr->string, "qw")) box->quat.w = attr->valuedouble;
		else if (STR_EQ(attr->string, "flags")) box->flags = attr->valueint;

		attr = attr->next;
	}

	return box;
}

cylinder_t* load_cylinder(cJSON* attr) {
	cylinder_t* cylinder = malloc(sizeof(cylinder_t));

	while (attr) {
		if (STR_EQ(attr->string, "id")) strcpy(cylinder->id, attr->valuestring);
		else if (STR_EQ(attr->string, "ax")) cylinder->a.x = attr->valuedouble;
		else if (STR_EQ(attr->string, "ay")) cylinder->a.y = attr->valuedouble;
		else if (STR_EQ(attr->string, "az")) cylinder->a.z = attr->valuedouble;
		else if (STR_EQ(attr->string, "bx")) cylinder->b.x = attr->valuedouble;
		else if (STR_EQ(attr->string, "by")) cylinder->b.y = attr->valuedouble;
		else if (STR_EQ(attr->string, "bz")) cylinder->b.z = attr->valuedouble;
		else if (STR_EQ(attr->string, "r")) cylinder->radius = attr->valuedouble;
		else if (STR_EQ(attr->string, "flags")) cylinder->flags = attr->valueint;
		else if (STR_EQ(attr->string, "interactable")) {
			cylinder->interaction = (interaction_t) {
				.enabled = true
			};
		} else if (cylinder->interaction.enabled) {
			if (STR_EQ(attr->string, "label")) strcpy(cylinder->interaction.label, attr->valuestring);
		}
		// todo: make an interaction object on collision json

		attr = attr->next;
	}

	return cylinder;
}

void scene_load_collisions(scene_t* scene) {
	int json_size;
	char* file = asset_load(scene->collision_path, &json_size);
	cJSON* json = cJSON_ParseWithLength(file, json_size);

	box_t* boxes[128];
	int boxes_count = 0;

	cylinder_t* cylinders[128];
	int cylinders_count = 0;

	int i = 0;
	cJSON* col = json->child;
	while (col) {
		cJSON* attr = col->child;
		assertf(STR_EQ(attr->string, "shape"), "First key of collision %d must be \"shape\"", i);

		if (STR_EQ(attr->valuestring, "box")) {
			boxes[boxes_count] = load_box(attr);
			boxes_count++;
		} else if (STR_EQ(attr->valuestring, "cylinder")) {
			cylinders[cylinders_count] = load_cylinder(attr);
			cylinders_count++;
		}

		i++;
		col = col->next;
	};

	scene->collision_boxes = malloc(sizeof(box_t*) * boxes_count);
	memcpy(scene->collision_boxes, boxes, sizeof(box_t*) * boxes_count);
	scene->collision_boxes_count = boxes_count;

	scene->collision_cylinders = malloc(sizeof(cylinder_t*) * cylinders_count);
	memcpy(scene->collision_cylinders, cylinders, sizeof(cylinder_t*) * cylinders_count);
	scene->collision_cylinders_count = cylinders_count;

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
