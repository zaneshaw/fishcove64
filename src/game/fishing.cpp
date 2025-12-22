#include "fishing.h"

#include "../util/constants.h"

#include <debug.h>

extern "C" {
	#include "../lib/pcg/pcg_basic.h"
}

pcg32_random_t* rng = 0x00;

// probably bad idk
void fishing_roll(fish_instance_t* fish_instance, fish_lt_t* lt) {
	if (!rng) {
		rng = (pcg32_random_t*) malloc(sizeof(pcg32_random_t));
		pcg32_srandom_r(rng, 0, getentropy32());
	}

	int weight_sum_max = 0;
	for (int i = 0; i < lt->count; i++) {
		weight_sum_max += lt->entries[i].rng_weight;
	}

	uint32_t rand = pcg32_boundedrand_r(rng, weight_sum_max);

	int fish_index = 0;
	int weight_sum = 0;
	for (int i = 0; i < lt->count; i++) {
		weight_sum += lt->entries[i].rng_weight;
		if (rand < (uint32_t) weight_sum) {
			fish_index = i;
			break;
		}
	}

	*fish_instance = (fish_instance_t) {
		.fish = fish_find(lt->entries[fish_index].fish_identifier),
		.length = 0.5f,
	};
}
