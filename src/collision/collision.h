#ifndef COLLISION_H_
#define COLLISION_H_

#include "fgeom.h"

typedef struct vec2_s {
	float x;
	float y;
} vec2_t;

typedef struct plane_s {
	fm_vec3_t normal;
	float offset;
} plane_t;

typedef struct ray_s {
	fm_vec3_t origin;
	fm_vec3_t direction;
} ray_t;

float triangle_sign2(vec2_t p1, vec2_t p2, vec2_t p3);
bool overlap_point2d_triangle2d(vec2_t point, fm_vec3_t v1, fm_vec3_t v2, fm_vec3_t v3);
float intersect_ray3d_plane3d(ray_t ray, plane_t plane);

#endif // COLLISION_H_
