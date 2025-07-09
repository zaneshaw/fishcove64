#ifndef SHAPE_BOX_H_
#define SHAPE_BOX_H_

#include "../../math/vector3.h"

#include <ccd/vec3.h>

typedef struct box_s {
	char id[16];

	vector3_t pos;
	vector3_t half_extents;
	fm_quat_t quat;
} box_t;

void box_support_function(const void* obj, const ccd_vec3_t* dir, ccd_vec3_t* vec);
void box_center_function(const void* obj, ccd_vec3_t* center);

#endif // SHAPE_BOX_H_
