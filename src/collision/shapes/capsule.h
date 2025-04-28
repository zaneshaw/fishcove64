#ifndef SHAPE_CAPSULE_H_
#define SHAPE_CAPSULE_H_

#include "../../interaction.h"
#include "../../math/vector3.h"
#include "../collision.h"

#include <ccd/vec3.h>

typedef struct capsule_s {
	vector3_t pos;
	float half_len;
	float radius;

	collision_mode_t mode;
	interaction_t interaction;
} capsule_t;

void capsule_support_function(const void* obj, const ccd_vec3_t* dir, ccd_vec3_t* vec);
void capsule_center_function(const void* obj, ccd_vec3_t* center);

#endif // SHAPE_CAPSULE_H_
