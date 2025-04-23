#include "player.h"

#include "../util/macros.h"

#include <debug.h>
#include <t3d/t3dmath.h>

#define PLAYER_MOVE_SPEED 5.0f
#define PLAYER_LOOK_SPEED 0.035f
#define PITCH_LERP_SPEED 10.0f

#define PITCH_HIGHEST T3D_DEG_TO_RAD(-60)
#define PITCH_HIGH T3D_DEG_TO_RAD(-30)
#define PITCH_MID 0
#define PITCH_LOW T3D_DEG_TO_RAD(30)
#define PITCH_LOWEST T3D_DEG_TO_RAD(60)

int current_pitch_mode = 0;
float pitch_modes[] = { PITCH_LOWEST, PITCH_LOW, PITCH_MID, PITCH_HIGH, PITCH_HIGHEST };

player_t player = (player_t) {
	.transform = (transform_t) {
		.position = { 0, 0, 0 },
		.rotation = { 0, 0, 0 },
		.scale = { 1, 1, 1 },
	},
	.camera_transform = (transform_t) {
		.position = { 0, 180, 0 },
		.rotation = { 0, 0, 0 },
		.scale = { 1, 1, 1 },
	},
	.look_speed = 10,
	.move_speed = 10,
};

// stick x 	look	left/right
// stick y 	move 	forward/back
// c x 		move	strafe
// c y 		look	pitch

void player_look(float delta_time) {
	if (JOYPAD_IS_READY) {
		joypad_inputs_t inputs = joypad_get_inputs(JOYPAD);
		joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD);

		// yaw
		player.camera_transform.rotation.y -= inputs.stick_x * PLAYER_LOOK_SPEED * delta_time;
		player.camera_transform.rotation.y = fm_fmodf(player.camera_transform.rotation.y + M_PI, M_TWOPI) - M_PI;

		// pitch
		if (pressed.c_up && current_pitch_mode < 2)
			current_pitch_mode++;
		else if (pressed.c_down && current_pitch_mode > -2)
			current_pitch_mode--;

		player.camera_transform.rotation.x = fm_lerp(player.camera_transform.rotation.x, pitch_modes[current_pitch_mode + 2], PITCH_LERP_SPEED * delta_time);
	}
}

void player_move(float delta_time) {
	if (JOYPAD_IS_READY) {
		joypad_inputs_t inputs = joypad_get_inputs(JOYPAD);

		// move
		player.transform.position.x += inputs.stick_y * fm_sinf(player.camera_transform.rotation.y) * PLAYER_MOVE_SPEED * delta_time;
		player.transform.position.z += inputs.stick_y * fm_cosf(player.camera_transform.rotation.y) * PLAYER_MOVE_SPEED * delta_time;

		// strafe
		player.transform.position.x -= inputs.cstick_x * fm_cosf(player.camera_transform.rotation.y) * PLAYER_MOVE_SPEED * delta_time;
		player.transform.position.z += inputs.cstick_x * fm_sinf(player.camera_transform.rotation.y) * PLAYER_MOVE_SPEED * delta_time;
	}
}

transform_t player_get_eye() {
	return (transform_t) {
		.position = {
			player.transform.position.x + player.camera_transform.position.x,
			player.transform.position.y + player.camera_transform.position.y,
			player.transform.position.z + player.camera_transform.position.z,
		},
		.rotation = {
			player.transform.rotation.x + player.camera_transform.rotation.x,
			player.transform.rotation.y + player.camera_transform.rotation.y,
			player.transform.rotation.z + player.camera_transform.rotation.z,
		},
	};
}
