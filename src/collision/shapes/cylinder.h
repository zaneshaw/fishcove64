#ifndef SHAPE_CYLINDER_H_
#define SHAPE_CYLINDER_H_

#include "../../math/vector3.h"
#include "../collision.h"

typedef struct cylinder_s {
	char id[16];

	fm_vec3_t a;
	fm_vec3_t b;
	float radius;

	collision_flags_t flags;
	interaction_t interaction;
} cylinder_t;

#endif // SHAPE_CYLINDER_H_
