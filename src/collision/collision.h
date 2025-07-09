#ifndef COLLISION_H_
#define COLLISION_H_

#include <ccd/ccd.h>

// todo: use bit flags
typedef enum {
	COLLISION_FLAG_NONE,
	COLLISION_FLAG_COLLIDE = 1 << 0, // block player movement
	COLLISION_FLAG_INTERACT = 1 << 1, // block rays
} collision_flags_t;

typedef enum {
	COLLISION_SHAPE_NONE,
	COLLISION_SHAPE_BOX,
	COLLISION_SHAPE_CAPSULE,
	COLLISION_SHAPE_CYLINDER,
} collision_shapes_t;

typedef struct collision_s {
	collision_shapes_t type;
	void* shape;
} collision_t;

extern ccd_t ccd;

void collision_init();

#endif // COLLISION_H_
