#include "fishing.h"

#include "../lib/pcg/pcg_basic.h"

#include <debug.h>

pcg32_random_t* rng = 0x00;

// todo: move to .json file
fish_t fish[3] = {
	{
		.name = "A fish",
		.s_name = "afishicus",
		.base_sell = 10,
		.length_min = 0.2f,
		.length_max = 1.2f,
		.water = WATER_TYPE_POND | WATER_TYPE_BEACH,
		.time_of_day = TOD_STATE_DAY,
		.rng_weight = 5,
		.sprite_path = "rom:/sprites/fish/fish001.sprite",
	},
	{
		.name = "B fish",
		.s_name = "bfishicus",
		.base_sell = 10,
		.length_min = 0.2f,
		.length_max = 1.2f,
		.water = WATER_TYPE_POND | WATER_TYPE_BEACH,
		.time_of_day = TOD_STATE_DAY,
		.rng_weight = 5,
		.sprite_path = "rom:/sprites/fish/fish002.sprite",
	},
	{
		.name = "Rare fish",
		.s_name = "rarefishicus",
		.base_sell = 50,
		.length_min = 0.5f,
		.length_max = 2.2f,
		.water = WATER_TYPE_POND | WATER_TYPE_BEACH,
		.time_of_day = TOD_STATE_DAY,
		.rng_weight = 1,
		.sprite_path = "rom:/sprites/fish/fish003.sprite",
	}
};

void fishing_init() {
	for (int i = 0; i < 3; i++) {
		fish[i].sprite = sprite_load(fish[i].sprite_path);
	}
}

// probably bad idk
void fishing_roll(fish_instance_t* fish_instance) {
	if (!rng) {
		rng = malloc(sizeof(pcg32_random_t));
		pcg32_srandom_r(rng, 0, getentropy32());
	}

	int weight_sum_max = 0;
	for (int i = 0; i < 3; i++) {
		weight_sum_max += fish[i].rng_weight;
	}

	uint32_t rand = pcg32_boundedrand_r(rng, weight_sum_max);

	int fish_index = 0;
	int weight_sum = 0;
	for (int i = 0; i < 3; i++) {
		weight_sum += fish[i].rng_weight;
		if (rand < weight_sum) {
			fish_index = i;
			break;
		}
	}

	*fish_instance = (fish_instance_t) {
		.fish = &fish[fish_index],
		.length = 0.5f,
	};
}

void fishing_debug(bool csv) {
	int weight_sum_max = 0;
	for (int i = 0; i < 3; i++) {
		weight_sum_max += fish[i].rng_weight;
	}

	if (csv) debugf("fish, chance\n");
	for (int i = 0; i < 3; i++) {
		if (csv) {
			debugf("%s, %f\n", fish[i].name, (float) fish[i].rng_weight / weight_sum_max);
		} else {
			debugf("%s: %.2f%%\n", fish[i].name, ((float) fish[i].rng_weight / weight_sum_max) * 100.0f);
		}
	}
}
