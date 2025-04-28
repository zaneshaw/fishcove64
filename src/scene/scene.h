#ifndef SCENE_H_
#define SCENE_H_

#include "../actor/actor.h"
#include "../collision/shapes/box.h"
#include "../collision/shapes/cylinder.h"

typedef struct scene_s scene_t;
typedef struct scene_s {
	const char* name;

	void (*load)(scene_t* this);
	void (*update)(scene_t* this, float delta_time, float elapsed);
	void (*render)(scene_t* this);

	color_t clear_color;
	const uint8_t* ambient_color;

	bool fog_enabled;
	color_t fog_color;
	int fog_near;
	int fog_far;

	actor_t** _actors;
	int _actor_count;
	int _actor_capacity;

	char* collision_path;

	box_t* collision_boxes;
	int collision_boxes_count;

	cylinder_t* collision_cylinders;
	int collision_cylinders_count;
} scene_t;

extern scene_t* current_scene;

void scene_add_box_collisions(scene_t* scene, box_t* boxes, int length);

void scene_load(scene_t* scene);
void scene_update(float delta_time, float elapsed);
void scene_render();
void scene_kill(scene_t* scene);

void scene_add_actor(scene_t* scene, actor_t* actor);

#endif // SCENE_H_
