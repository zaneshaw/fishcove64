#include "save.h"

#include "../player/player.h"

#include <libdragon.h>

void load(save_data_t* data) {
	save_data_t _data __attribute__((aligned(16)));

	data_cache_hit_invalidate(&_data, sizeof(_data));
	dma_read_raw_async(&_data, SRAM_BASE_ADDRESS, sizeof(_data));
	dma_wait();

	player.transform.position.x = _data.player_pos_x;
	player.transform.position.y = _data.player_pos_y;
	player.transform.position.z = _data.player_pos_z;
	player.camera_transform.rotation.y = _data.player_yaw;
	player.camera_transform.rotation.y = _data.player_yaw;

	*data = _data;
}

void save(float elapsed) {
	save_data_t _data __attribute__((aligned(16)));

	_data.player_pos_x = player.transform.position.x;
	_data.player_pos_y = player.transform.position.y;
	_data.player_pos_z = player.transform.position.z;
	_data.player_yaw = player.camera_transform.rotation.y;
	_data.elapsed = elapsed;

	data_cache_hit_writeback(&_data, sizeof(_data));
	dma_write_raw_async(&_data, SRAM_BASE_ADDRESS, sizeof(_data));
	dma_wait();
}
