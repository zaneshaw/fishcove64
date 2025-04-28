#ifndef COLLISION_H_
#define COLLISION_H_

#include <ccd/ccd.h>

typedef enum {
	COLLISION_MODE_BOTH,
	COLLISION_MODE_COLLIDE, // block player movement
	COLLISION_MODE_INTERACT, // block rays
	COLLISION_MODE_NONE,
} collision_mode_t;

typedef enum {
	COLLISION_SHAPE_NONE,
	COLLISION_SHAPE_BOX,
	COLLISION_SHAPE_CAPSULE,
	COLLISION_SHAPE_CYLINDER,
} collision_shapes_t;

extern ccd_t ccd;

void collision_init();

#endif // COLLISION_H_
