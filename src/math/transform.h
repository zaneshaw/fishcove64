#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <fgeom.h>

typedef struct transform_s {
	fm_vec3_t position;
	fm_vec3_t rotation;
	fm_vec3_t scale;
} transform_t;

#endif // TRANSFORM_H_
