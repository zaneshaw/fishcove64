#ifndef SAVE_H_
#define SAVE_H_

#define SRAM_BASE_ADDRESS 0x08000000

typedef struct __attribute__((aligned(16))) {
	float player_pos_x;
	float player_pos_y;
	float player_pos_z;
	float player_yaw;
	float elapsed;
} save_data_t;

void load(save_data_t* data);
void save(float elapsed);

#endif // SAVE_H_
