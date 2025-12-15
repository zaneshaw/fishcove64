#ifndef DEFINES_H_
#define DEFINES_H_

#include <joypad.h>

// should probably move some constants to the files they actually belong in

// https://github.com/DragonMinded/libdragon/blob/trunk/src/utils.h#L12
#define MAX(a,b) ({ typeof(a) _a = a; typeof(b) _b = b; _a > _b ? _a : _b; })
#define MIN(a,b) ({ typeof(a) _a = a; typeof(b) _b = b; _a < _b ? _a : _b; })
#define CLAMP(x, min, max) (MIN(MAX((x), (min)), (max)))

#define OVERSCAN_PAD_X (int) 20
#define OVERSCAN_PAD_Y (int) 15

#define WORLD_SCALE 100.0f / 256.0f
#define FOV 70.0f

#define NEAR_PLANE 50.0f
#define FAR_PLANE 1500.0f
#define GL_NEAR_PLANE 20.0f
#define GL_FAR_PLANE 2000.0f

#define BOX_DEBUG_LEVEL BOX_DEBUG_NONE

#define PLAYER_REACH 150.0f

#define JOYPAD JOYPAD_PORT_1
#define JOYPAD_IS_READY joypad_is_connected(JOYPAD) && joypad_get_identifier(JOYPAD) == JOYBUS_IDENTIFIER_N64_CONTROLLER

#define STR_EQ(a, b) (strcmp(a, b) == 0)
#define ARR_LEN(arr, type) (sizeof(arr) / sizeof(type))

#endif // DEFINES_H_
