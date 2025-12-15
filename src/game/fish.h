#ifndef FISH_H_
#define FISH_H_

#include <libdragon.h>

typedef struct fish_s {
	const char* identifier;
	const char* name;
	const char* s_name;
	float base_sell;
	float length_min;
	float length_max;
	// tackle

	const char* sprite_path;
	sprite_t* sprite; // todo: do cache
} fish_t;

extern fish_t fish[];

void fish_init();
fish_t* fish_find(const char* identifier);

#endif // FISH_H_
