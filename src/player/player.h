#ifndef PLAYER_H_
#define PLAYER_H_

#include "../math/transform.h"
#include "../collision/shapes/capsule.h"

typedef struct player_s {
	transform_t transform;
	transform_t camera_transform;

	capsule_t capsule;

	float height;
	float radius;

	float look_speed;
	float move_speed;
} player_t;

extern player_t player;

void player_look(float delta_time);
void player_move(float delta_time);
void player_interact();
transform_t player_get_eye();

#endif // PLAYER_H_
