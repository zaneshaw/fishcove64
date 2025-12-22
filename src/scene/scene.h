#ifndef SCENE_H_
#define SCENE_H_

#include "../actor/actor.h"

typedef struct collision_vertex_s {
	fm_vec3_t pos;
} collision_vertex_t;

typedef struct collision_tri_s {
	uint16_t v1;
	uint16_t v2;
	uint16_t v3;
	uint16_t normal;
} collision_tri_t;

typedef struct collision_mesh_s {
	fm_vec3_t* vertices;
	uint16_t vertex_count;

	fm_vec3_t* normals;
	uint16_t normal_count;

	collision_tri_t* tris;
	uint16_t tri_count;
} collision_mesh_t;

typedef struct collision_s {
	const char* path;
	collision_mesh_t mesh;
} collision_t;

typedef struct scene_s scene_t;
typedef struct scene_s {
	const char* name;

	color_t clear_color;
	const uint8_t* ambient_color;
	const uint8_t* sun_color;
	fm_vec3_t sun_dir;

	bool fog_enabled;
	color_t fog_color;
	float fog_near;
	float fog_far;

	collision_t collision;

	vector3_t spawn_position;
	float spawn_yaw;

	actor_t** _actors;
	int _actor_count;
	int _actor_capacity;

	void (*load)(scene_t* self);
	void (*update)(scene_t* self, float delta_time, float elapsed);
	void (*render)(scene_t* self);
} scene_t;

extern scene_t scene_playground;
extern scene_t scene_area1;
extern scene_t scene_collision_test;

void scene_add_actor(scene_t* scene, actor_t* actor);

void scene_update(float delta_time, float elapsed);
void scene_render();
void scene_kill(scene_t* scene);

#endif // SCENE_H_
