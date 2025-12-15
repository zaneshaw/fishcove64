#ifndef FISHING_H_
#define FISHING_H_

#include "fish.h"

#include <libdragon.h>
#include <stdbool.h>
#include <stdint.h>

#define INVENTORY_SLOTS 12

typedef struct fish_lt_entry_s {
	const char* fish_identifier;
	uint8_t rng_weight;
} fish_lt_entry_t;

// loot table
typedef struct fish_lt_s {
	fish_lt_entry_t* entries;
	int count;
} fish_lt_t;

typedef struct fish_instance_s {
	fish_t* fish;
	float length;
} fish_instance_t;

void fishing_roll(fish_instance_t* fish_instance, fish_lt_t* lt);

#endif // FISHING_H_
