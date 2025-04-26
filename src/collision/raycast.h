#ifndef RAYCAST_H_
#define RAYCAST_H_

#include "../math/vector3.h"
#include "shapes/box.h"
#include "shapes/cylinder.h"

typedef struct ray_s {
	fm_vec3_t origin;
	fm_vec3_t dir;
} ray_t;

typedef struct ray_intersect_s {
	bool hit;
	float dist;
	fm_vec3_t point;
} ray_intersect_t;

void raycast_cylinder(ray_intersect_t* ray_intersect, ray_t* ray, cylinder_t* cylinder);
// void raycast_box(ray_intersect_t* ray_intersect, ray_t* ray, box_t* box, float distance);

#endif // RAYCAST_H_
