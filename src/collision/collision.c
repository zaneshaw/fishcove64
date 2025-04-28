#include "collision.h"

#include <ccd/quat.h>
#include <ccd/vec3.h>

ccd_t ccd;

void collision_init() {
	CCD_INIT(&ccd);

	ccd.max_iterations = 100;
	ccd.mpr_tolerance = 0.1;
}
