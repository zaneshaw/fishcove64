#ifndef WATER_H_
#define WATER_H_

#include "fishing.h"

#include <stdint.h>

typedef struct water_s {
	fish_lt_t lt_day;
	fish_lt_t lt_night;

	float frenzy;
} water_t;

void water_catch(water_t* water);

#endif // WATER_H_
