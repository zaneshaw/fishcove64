#include "fish.h"

#include "../util/constants.h"

fish_t fish[] = {
	{
		.identifier = "dead_fish",
		.name = "dead fish",
		.s_name = "dead fishicus",
		.base_sell = 0,
		.length_min = 6.66f,
		.length_max = 6.66f,
		.sprite_path = "rom:/sprites/fish/fish001.sprite",
	},
	{
		.identifier = "a_fish",
		.name = "A fish",
		.s_name = "afishicus",
		.base_sell = 10,
		.length_min = 0.2f,
		.length_max = 1.2f,
		.sprite_path = "rom:/sprites/fish/fish001.sprite",
	},
	{
		.identifier = "b_fish",
		.name = "B fish",
		.s_name = "bfishicus",
		.base_sell = 10,
		.length_min = 0.2f,
		.length_max = 1.2f,
		.sprite_path = "rom:/sprites/fish/fish002.sprite",
	},
	{
		.identifier = "rare_fish",
		.name = "Rare fish",
		.s_name = "rarefishicus",
		.base_sell = 50,
		.length_min = 0.5f,
		.length_max = 2.2f,
		.sprite_path = "rom:/sprites/fish/fish003.sprite",
	}
};

void fish_init() {
	for (int i = 0; i < ARR_LEN(fish, fish_t); i++) {
		fish[i].sprite = sprite_load(fish[i].sprite_path);
	}
}

fish_t* fish_find(const char* identifier) {
	fish_t* _fish = 0x00;
	for (int i = 0; i < ARR_LEN(fish, fish_t); i++) {
		if (STR_EQ(fish[i].identifier, identifier)) {
			_fish = &fish[i];
			break;
		}
	}

	if (_fish == 0x00) {
		debugf("fish does not exist with id '%s'", identifier);
		return &fish[0];
	}

	return _fish;
}
