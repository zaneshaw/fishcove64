#include "player.h"

#include "../collision/collision.h"
#include "../collision/raycast.h"
#include "../collision/shapes/box.h"
#include "../collision/shapes/capsule.h"
#include "../font/font.h"
#include "../game/fishing.h"
#include "../game/game.h"
#include "../lib/debug.h"
#include "../math/lerp.h"
#include "../scene/scene_loader.h"
#include "../util/constants.h"
#include "inventory.h"

#include <debug.h>
#include <t3d/t3dmath.h>

#define PLAYER_LOOK_SPEED 0.035f
#define PITCH_LERP_SPEED 10.0f

#define PLAYER_DEFAULT_POSITION ((vector3_t) { 0, 100, -300 }) // collision bugs out at 0, 0, 0
#define PLAYER_DEFAULT_YAW 0
#define PLAYER_DEFAULT_PITCH 3

#define DEADZONE_X 2.5f
#define DEADZONE_Y 2.5f

float player_move_speed = 3.0f;

int current_pitch_mode = PLAYER_DEFAULT_PITCH;
float pitch_modes[] = {
	T3D_DEG_TO_RAD(-75),
	T3D_DEG_TO_RAD(-50),
	T3D_DEG_TO_RAD(-25),
	T3D_DEG_TO_RAD(0),
	T3D_DEG_TO_RAD(25),
	T3D_DEG_TO_RAD(50),
	T3D_DEG_TO_RAD(75),
};

actor_t* target_actor = NULL;

rdpq_textparms_t interaction_label_params;

player_t player = (player_t) {
	.transform = (transform_t) {
		.position = PLAYER_DEFAULT_POSITION,
		.rotation = { 0, PLAYER_DEFAULT_YAW, 0 },
		.scale = { 1, 1, 1 },
	},
	.camera_transform = (transform_t) {
		.position = { 0, 180, 0 },
		.rotation = { 0, 0, 0 },
		.scale = { 1, 1, 1 },
	},

	.height = 180,
	.radius = 25,

	.look_speed = 10,
	.move_speed = 10,
};

void player_reset() {
	player.transform.position = PLAYER_DEFAULT_POSITION;
	player.transform.rotation.y = PLAYER_DEFAULT_YAW; // just doesn't work sometimes?!
	current_pitch_mode = PLAYER_DEFAULT_PITCH;
}

void player_update_capsule() {
	player.capsule.pos = (vector3_t) { player.transform.position.x, player.transform.position.y + player.height / 2.0f, player.transform.position.z };
	player.capsule.half_len = player.height / 2.0f - player.radius;
	player.capsule.radius = player.radius;
}

void player_collide_world() {
	player_update_capsule();

	ccd.support1 = capsule_support_function;
	ccd.center1 = capsule_center_function;

	// todo: expand to support multiple collision shapes
	ccd.support2 = box_support_function;
	ccd.center2 = box_center_function;

	float depth;
	ccd_vec3_t dir;
	ccd_vec3_t pos;

	for (int i = 0; i < current_scene->_actor_count; i++) {
		for (int j = 0; j < current_scene->_actors[i]->collision_count; j++) {
			collision_t* col = &current_scene->_actors[i]->collisions[j];
			if ((col->flags & COLLISION_FLAG_COLLIDE) == COLLISION_FLAG_COLLIDE) {
				box_t* box = col->shape;
				int intersect = ccdMPRPenetration(&player.capsule, box, &ccd, &depth, &dir, &pos);
				if (intersect == 0) {
					player.transform.position.x -= dir.v[0] * depth;
					player.transform.position.y -= dir.v[1] * depth;
					player.transform.position.z -= dir.v[2] * depth;
				}
			}
		}
	}
}

void player_raycast_world() {
	transform_t eye = player_get_eye();
	fm_vec3_t eye_forward = { {
		fm_cosf(eye.rotation.x) * fm_sinf(eye.rotation.y),
		-fm_sinf(eye.rotation.x),
		fm_cosf(eye.rotation.x) * fm_cosf(eye.rotation.y),
	} };

	ray_intersect_t ray_intersect;
	ray_t ray = {
		.origin = vector3_to_fgeom(eye.position),
		.dir = eye_forward,
	};

	float closest = PLAYER_REACH;
	target_actor = NULL;
	for (int i = 0; i < current_scene->_actor_count; i++) {
		for (int j = 0; j < current_scene->_actors[i]->collision_count; j++) {
			collision_t* col = &current_scene->_actors[i]->collisions[j];
			if ((col->flags & COLLISION_FLAG_INTERACT) == COLLISION_FLAG_INTERACT) {
				if (col->type == COLLISION_SHAPE_CYLINDER) {
					cylinder_t* cylinder = col->shape;
					raycast_cylinder(&ray_intersect, &ray, cylinder);
					if (ray_intersect.dist < closest) {
						closest = ray_intersect.dist;
						target_actor = current_scene->_actors[i];
					}
				}
			}
		}
	}
}

char* command_speed() {
	debugf("aaaa\n"); // doesn't work on usb/hardware
	int size = debug_sizecommand();
	char speed[4];

	if (size == 0) {
		return "must specify speed! try 'speed 5'";
	} else if (size > 2) {
		return "specified speed is too high! below 100.";
	} else {
		debug_parsecommand(speed);
		speed[size] = '\0';
	}

	char* output = (char*) malloc(100);
	sprintf(output, "speed set from %.2f to %d.00!", player_move_speed, atoi(speed));

	player_move_speed = atoi(speed);

	return output;
}

void player_init() {
	debug_addcommand("speed", "sets the player's speed\n\texample usage: speed 5", command_speed);

	interaction_label_params = (rdpq_textparms_t) {
		.width = (short) display_get_width(),
		.height = (short) display_get_height(),
		.align = ALIGN_CENTER,
		.valign = VALIGN_CENTER,
	};
}

void player_look(float delta_time) {
	if (game_input_state == GAME_INPUT_NORMAL && JOYPAD_IS_READY) {
		joypad_inputs_t inputs = joypad_get_inputs(JOYPAD);
		joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD);

		if (fabsf(inputs.stick_x) > DEADZONE_X) {
			// yaw
			player.camera_transform.rotation.y -= inputs.stick_x * PLAYER_LOOK_SPEED * delta_time;
			player.camera_transform.rotation.y = fm_fmodf(player.camera_transform.rotation.y + M_PI, M_TWOPI) - M_PI;
		}

		// pitch
		if (pressed.c_up && current_pitch_mode > 0)
			current_pitch_mode--;
		else if (pressed.c_down && current_pitch_mode < (sizeof(pitch_modes) / sizeof(int)) - 1)
			current_pitch_mode++;

		player.camera_transform.rotation.x = lerp(
			player.camera_transform.rotation.x,
			pitch_modes[current_pitch_mode],
			PITCH_LERP_SPEED * delta_time,
			0.001f
		);
	}
}

void player_move(float delta_time) {
	if (game_input_state == GAME_INPUT_NORMAL && JOYPAD_IS_READY) {
		joypad_inputs_t inputs = joypad_get_inputs(JOYPAD);

		if (fabsf(inputs.stick_y) > DEADZONE_Y) {
			// move
			player.transform.position.x += inputs.stick_y * fm_sinf(player.camera_transform.rotation.y) * player_move_speed * delta_time;
			player.transform.position.z += inputs.stick_y * fm_cosf(player.camera_transform.rotation.y) * player_move_speed * delta_time;
		}

		// strafe
		player.transform.position.x += inputs.cstick_x * -fm_cosf(player.camera_transform.rotation.y) * player_move_speed * delta_time;
		player.transform.position.z += inputs.cstick_x * fm_sinf(player.camera_transform.rotation.y) * player_move_speed * delta_time;
	}

	player.transform.position.y -= 200 * delta_time; // todo: ground check and velocity

	player_collide_world();
	player_raycast_world();

	if (
		fabsf(player.transform.position.x) > 15000.0f ||
		fabsf(player.transform.position.y) > 15000.0f ||
		fabsf(player.transform.position.z) > 15000.0f
	) {
		player_reset();
	}
}

void player_interact() {
	if (game_input_state == GAME_INPUT_NORMAL && JOYPAD_IS_READY) {
		joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD);

		if (target_actor != NULL && target_actor->interact && pressed.a) {
			target_actor->interact(target_actor);
		}
	}
}

void player_render() {
	inventory_render();

	if (target_actor != NULL && !STR_EQ(target_actor->label, "")) {
		rdpq_sync_pipe();
		rdpq_mode_push();

		rdpq_set_mode_standard();

		rdpq_text_printf(&interaction_label_params, FONT_NORMAL, 0, 0, target_actor->label);

		rdpq_mode_pop();
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
