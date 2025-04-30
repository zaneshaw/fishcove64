#ifndef FISHING_H_
#define FISHING_H_

#include <libdragon.h>
#include <stdbool.h>
#include <stdint.h>

#define INVENTORY_SLOTS 12

typedef enum {
	WATER_TYPE_NONE,
	WATER_TYPE_POND = 1 << 0,
	WATER_TYPE_BEACH = 1 << 1,
} water_type_t;

typedef enum {
	TOD_STATE_NONE0,
	TOD_STATE_DAY = 1 << 0,
	TOD_STATE_NIGHT = 1 << 1,
} tod_t;

typedef struct fish_s {
	const char* name;
	const char* s_name;
	float base_sell;
	float length_min;
	float length_max;
	uint8_t water;
	uint8_t time_of_day;
	// tackle
	uint8_t rng_weight;

	const char* sprite_path;
	sprite_t* sprite; // todo: do cache
} fish_t;

typedef struct fish_instance_s {
	fish_t* fish;
	float length;
} fish_instance_t;

void fishing_init();
void fishing_roll(fish_instance_t* fish_instance);
void fishing_debug(bool csv);

#endif // FISHING_H_
