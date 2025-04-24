#include "box.h"

#include <ccd/ccd.h>
#include <ccd/quat.h>

void box_support_function(const void* obj, const ccd_vec3_t* dir, ccd_vec3_t* vec) {
	const box_t* box = (const box_t*) obj;

	ccd_vec3_t pos;
	ccdVec3Set(&pos, box->pos.x, box->pos.y, box->pos.z);
	ccd_vec3_t half_extents;
	ccdVec3Set(&half_extents, box->half_extents.x, box->half_extents.y, box->half_extents.z);
	ccd_quat_t quat;
	ccdQuatSet(&quat, box->quat.x, box->quat.y, box->quat.z, box->quat.w);

	ccd_vec3_t local_dir;
	ccd_quat_t inv_quat;
	ccd_vec3_t local_support_point;
	ccd_vec3_t world_support_point;

	ccdQuatInvert2(&inv_quat, &quat);
	ccdVec3Copy(&local_dir, dir);
	ccdQuatRotVec(&local_dir, &inv_quat);

	ccdVec3Set(
		&local_support_point,
		ccdSign(ccdVec3X(&local_dir)) * ccdVec3X(&half_extents),
		ccdSign(ccdVec3Y(&local_dir)) * ccdVec3Y(&half_extents),
		ccdSign(ccdVec3Z(&local_dir)) * ccdVec3Z(&half_extents)
	);

	ccdVec3Copy(&world_support_point, &local_support_point);
	ccdQuatRotVec(&world_support_point, &quat);

	ccdVec3Copy(vec, &world_support_point);
	ccdVec3Add(vec, &pos);
}

void box_center_function(const void* obj, ccd_vec3_t* center) {
	const box_t* box = (const box_t*) obj;

	ccdVec3Set(center, box->pos.x, box->pos.y, box->pos.z);
}
