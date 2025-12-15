#include "water.h"

#include "../player/inventory.h"

void water_catch(water_t* water) {
	fish_instance_t fish_instance;
	fishing_roll(&fish_instance, &water->lt_day);
	inventory_add(&fish_instance);
}
