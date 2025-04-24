#include "collision.h"

#include <ccd/quat.h>
#include <ccd/vec3.h>

ccd_t ccd;
// void** collision_objects;
box_t** collision_boxes;
int collision_count;

void collision_init() {
	CCD_INIT(&ccd);

	ccd.max_iterations = 100;
	ccd.mpr_tolerance = 0.1;
}

void collision_allocate(int size) {
	collision_boxes = malloc(size * sizeof(box_t*));
}

void collision_add(box_t* box) {
	collision_boxes[collision_count] = box;
	collision_count++;
}

void collision_free() {
	free(collision_boxes);
	collision_count = 0;
}
