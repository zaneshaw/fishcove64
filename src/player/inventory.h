#ifndef INVENTORY_H_
#define INVENTORY_H_

#include "../game/fishing.h"

typedef struct slot_s {
	bool not_empty;
	fish_instance_t fish_instance;
} slot_t;

void inventory_add(fish_instance_t* fish_instance);
void inventory_render();
void inventory_debug();

#endif // INVENTORY_H_
