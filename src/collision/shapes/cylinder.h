#ifndef SHAPE_CYLINDER_H_
#define SHAPE_CYLINDER_H_

#include "../../math/vector3.h"

typedef struct cylinder_s {
	fm_vec3_t a;
	fm_vec3_t b;
	float radius;
} cylinder_t;

#endif // SHAPE_CYLINDER_H_
