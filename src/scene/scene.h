#ifndef SCENE_H_
#define SCENE_H_

#include "../actor/actor.h"

typedef struct scene_s scene_t;
typedef struct scene_s {
	const char* name;

	void(*load)(scene_t* this);
	void(*update)(scene_t* this, float delta_time, float elapsed);

	actor_t** _actors;
	int _actor_count;
	int _actor_capacity;
} scene_t;

void scene_load(scene_t* scene);
void scene_update(float delta_time, float elapsed);
void scene_render();
void scene_kill(scene_t* scene);

void scene_add_actor(scene_t* scene, actor_t* actor);

#endif // SCENE_H_
