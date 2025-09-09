#include "collision.h"

#include "debug.h"

float triangle_sign2(vec2_t p1, vec2_t p2, vec2_t p3) {
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

// https://stackoverflow.com/a/2049593/10851455
bool overlap_point2d_triangle2d(vec2_t point, fm_vec3_t v1, fm_vec3_t v2, fm_vec3_t v3) {
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = triangle_sign2(point, (vec2_t) { v1.x, v1.z }, (vec2_t) { v2.x, v2.z });
	d2 = triangle_sign2(point, (vec2_t) { v2.x, v2.z }, (vec2_t) { v3.x, v3.z });
	d3 = triangle_sign2(point, (vec2_t) { v3.x, v3.z }, (vec2_t) { v1.x, v1.z });

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

float intersect_ray3d_plane3d(ray_t ray, plane_t plane) {
	return -(fm_vec3_dot(&ray.origin, &plane.normal) - plane.offset) / fm_vec3_dot(&ray.direction, &plane.normal);
}
