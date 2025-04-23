#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "vector3.h"

typedef struct transform_s {
	vector3_t position;
	vector3_t rotation;
	vector3_t scale;
} transform_t;

#endif // TRANSFORM_H_
