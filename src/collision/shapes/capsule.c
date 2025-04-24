#include "capsule.h"

void capsule_support_function(const void* obj, const ccd_vec3_t* dir, ccd_vec3_t* vec) {
	const capsule_t* capsule = (const capsule_t*) obj;

	ccd_vec3_t p1;
	ccdVec3Set(&p1, capsule->pos.x, capsule->pos.y + capsule->half_len, capsule->pos.z);
	ccd_vec3_t p2;
	ccdVec3Set(&p2, capsule->pos.x, capsule->pos.y - capsule->half_len, capsule->pos.z);

	ccd_vec3_t segment_vec = p2;
	ccdVec3Sub(&segment_vec, &p1);

	ccd_real_t dot_p1 = ccdVec3Dot(dir, &p1);
	ccd_real_t dot_p2 = ccdVec3Dot(dir, &p2);

	ccd_vec3_t segment_support_point;
	if (dot_p1 > dot_p2) {
		ccdVec3Copy(&segment_support_point, &p1);
	} else {
		ccdVec3Copy(&segment_support_point, &p2);
	}

	ccd_vec3_t normalized_dir;
	ccdVec3Copy(&normalized_dir, dir);
	ccdVec3Normalize(&normalized_dir);

	ccd_vec3_t radius_offset = normalized_dir;
	ccdVec3Scale(&radius_offset, capsule->radius);

	*vec = segment_support_point;
	ccdVec3Add(vec, &radius_offset);
}

void capsule_center_function(const void* obj, ccd_vec3_t* center) {
	const capsule_t* capsule = (const capsule_t*) obj;

	ccdVec3Set(center, capsule->pos.x, capsule->pos.y, capsule->pos.z);
}
