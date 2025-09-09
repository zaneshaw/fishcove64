#include "player.h"

#include "../collision/collision.h"
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

bool player_collide_world(vector3_t pos) {
	vec2_t player_pos = { pos.x, pos.z };
	for (int i = 0; i < current_scene->collision.mesh.tri_count; i++) {
		bool intersect = overlap_point2d_triangle2d(
			player_pos,
			current_scene->collision.mesh.vertices[current_scene->collision.mesh.tris[i].v1],
			current_scene->collision.mesh.vertices[current_scene->collision.mesh.tris[i].v2],
			current_scene->collision.mesh.vertices[current_scene->collision.mesh.tris[i].v3]
		);

		if (intersect) {
			// https://iquilezles.org/articles/intersectors/
			ray_t ray;
			ray.origin = (fm_vec3_t) { { player.transform.position.x, player.transform.position.y, player.transform.position.z } };
			ray.direction = (fm_vec3_t) { { 0, -1, 0 } };

			fm_vec3_t* normal = &current_scene->collision.mesh.normals[current_scene->collision.mesh.tris[i].normal];

			plane_t plane;
			plane.normal = *normal;
			plane.offset = fm_vec3_dot(&current_scene->collision.mesh.vertices[current_scene->collision.mesh.tris[i].v1], normal);

			float distance = intersect_ray3d_plane3d(ray, plane);

			if (abs(distance) >= 10) {
				continue;
			}

			player.transform.position.y -= distance;
			return true;
		}
	}

	return false;
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

void player_reset() {
	player.transform.position = current_scene->spawn_position;
	player.camera_transform.rotation.y = current_scene->spawn_yaw;
	current_pitch_mode = PLAYER_DEFAULT_PITCH;
	player.camera_transform.rotation.x = pitch_modes[current_pitch_mode];
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
	vector3_t next = player.transform.position;

	if (game_input_state == GAME_INPUT_NORMAL && JOYPAD_IS_READY) {
		joypad_inputs_t inputs = joypad_get_inputs(JOYPAD);

		if (fabsf(inputs.stick_y) > DEADZONE_Y) {
			// move
			next.x += inputs.stick_y * fm_sinf(player.camera_transform.rotation.y) * player_move_speed * delta_time;
			next.z += inputs.stick_y * fm_cosf(player.camera_transform.rotation.y) * player_move_speed * delta_time;
		}

		// strafe
		next.x += inputs.cstick_x * -fm_cosf(player.camera_transform.rotation.y) * player_move_speed * delta_time;
		next.z += inputs.cstick_x * fm_sinf(player.camera_transform.rotation.y) * player_move_speed * delta_time;
	}

	// so sketchy. needs actual checks and corrections
	bool allowed_to_move_x = player_collide_world((vector3_t) { next.x, player.transform.position.y, player.transform.position.z });
	bool allowed_to_move_z = player_collide_world((vector3_t) { player.transform.position.x, player.transform.position.y, next.z });
	if (allowed_to_move_x) player.transform.position.x = next.x;
	if (allowed_to_move_z) player.transform.position.z = next.z;

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
