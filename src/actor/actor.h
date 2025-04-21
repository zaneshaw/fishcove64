#ifndef ACTOR_H_
#define ACTOR_H_

#include "../math/transform.h"

#include <t3d/t3dmodel.h>
#include <rspq.h>

typedef struct actor_s {
	transform_t transform;
	T3DMat4FP* transform_matrix;

	T3DModel* model;

	rspq_block_t* block;
} actor_t;

void actor_draw(actor_t* actor);
void actor_kill(actor_t* actor);

#endif // ACTOR_H_
