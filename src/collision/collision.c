#include "collision.h"

#include "shapes/box.h"
#include "shapes/cylinder.h"
#include <ccd/quat.h>
#include <ccd/vec3.h>
#include <string.h>

ccd_t ccd;

void collision_init() {
	CCD_INIT(&ccd);

	ccd.max_iterations = 100;
	ccd.mpr_tolerance = 0.1;
}

collision_t* collision_allocate(int count, collision_t* collisions) {
	collision_t* _collisions = malloc(sizeof(collision_t) * count);
	memcpy(_collisions, collisions, sizeof(collision_t) * count);

	for (int i = 0; i < count; i++) {
		size_t size = 0;

		if (collisions[i].type == COLLISION_SHAPE_BOX) {
			size = sizeof(box_t);
		} else if (collisions[i].type == COLLISION_SHAPE_CYLINDER) {
			size = sizeof(cylinder_t);
		}

		_collisions[i].shape = malloc(size);
		memcpy(_collisions[i].shape, collisions[i].shape, size);
	}

	return _collisions;
}
