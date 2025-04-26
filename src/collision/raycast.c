#include "raycast.h"

// https://iquilezles.org/articles/intersectors/
void raycast_cylinder(ray_intersect_t* ray_intersect, ray_t* ray, cylinder_t* cylinder) {
	*ray_intersect = (ray_intersect_t) { .hit = false };

	fm_vec3_t ba;
	fm_vec3_sub(&ba, &cylinder->b, &cylinder->a);

	fm_vec3_t oc;
	fm_vec3_sub(&oc, &ray->origin, &cylinder->a);

	float baba = fm_vec3_dot(&ba, &ba);
	float bard = fm_vec3_dot(&ba, &ray->dir);
	float baoc = fm_vec3_dot(&ba, &oc);

	float k2 = baba - bard * bard;
	float k1 = baba * fm_vec3_dot(&oc, &ray->dir) - baoc * bard;
	float k0 = baba * fm_vec3_dot(&oc, &oc) - baoc * baoc - cylinder->radius * cylinder->radius * baba;

	float h = k1 * k1 - k2 * k0;

	if (h < 0.0) return;

	h = sqrt(h);
	float t = (-k1 - h) / k2;

	// body
	float y = baoc + t * bard;
	if (y > 0.0 && y < baba) {
		fm_vec3_t point;
		fm_vec3_scale(&point, &ray->dir, t);
		fm_vec3_add(&point, &point, &ray->origin);

		*ray_intersect = (ray_intersect_t) {
			.hit = true,
			.dist = t,
			.point = point,
		};
	}

	// caps
	if (fabsf(bard) >= FM_EPSILON) {
		t = (((y < 0.0) ? 0.0 : baba) - baoc) / bard;
		if (fabsf(k1 + k2 * t) < h) {
			fm_vec3_t point;
			fm_vec3_scale(&point, &ray->dir, t);
			fm_vec3_add(&point, &point, &ray->origin);

			*ray_intersect = (ray_intersect_t) {
				.hit = true,
				.dist = t,
				.point = point,
			};
		}
	}
}
