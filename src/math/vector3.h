#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "fgeom.h"

typedef struct vector3_s {
	float x, y, z;
} vector3_t;

extern vector3_t vector3_right;
extern vector3_t vector3_up;
extern vector3_t vector3_forward;
extern vector3_t vector3_zero;
extern vector3_t vector3_one;

#define vector3_to_fgeom(in) (fm_vec3_t) { { in.x, in.y, in.z } }
#define vector3_from_fgeom(in) (vector3_t) { { in.x, in.y, in.z } }

void vector3_add(vector3_t* out, vector3_t* a, vector3_t* b);
void vector3_subtract(vector3_t* out, vector3_t* a, vector3_t* b);
void vector3_multiply(vector3_t* out, vector3_t* a, vector3_t* b);
void vector3_divide(vector3_t* out, vector3_t* a, vector3_t* b);
void vector3_scale(vector3_t* out, vector3_t* a, float b);

#endif // VECTOR3_H_
