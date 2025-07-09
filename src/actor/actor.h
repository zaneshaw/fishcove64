#ifndef ACTOR_H_
#define ACTOR_H_

#include "../collision/collision.h"
#include "../interaction.h"
#include "../math/transform.h"

#include <rspq.h>
#include <t3d/t3dmodel.h>

typedef struct actor_s {
	transform_t transform;
	T3DMat4FP* transform_matrix;

	T3DModel* model;

	rspq_block_t* block;

	collision_t* collisions;
	int collision_count;

	interaction_t interaction;
} actor_t;

void actor_draw(actor_t* actor);
void actor_kill(actor_t* actor);

#endif // ACTOR_H_
