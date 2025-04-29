#ifndef DEFINES_H_
#define DEFINES_H_

#include <joypad.h>

// https://github.com/DragonMinded/libdragon/blob/trunk/src/utils.h#L12
#define MAX(a,b) ({ typeof(a) _a = a; typeof(b) _b = b; _a > _b ? _a : _b; })
#define MIN(a,b) ({ typeof(a) _a = a; typeof(b) _b = b; _a < _b ? _a : _b; })
#define CLAMP(x, min, max) (MIN(MAX((x), (min)), (max)))

#define OVERSCAN_PAD_X (int) 20
#define OVERSCAN_PAD_Y (int) 15

#define JOYPAD JOYPAD_PORT_1
#define JOYPAD_IS_READY joypad_is_connected(JOYPAD) && joypad_get_identifier(JOYPAD) == JOYBUS_IDENTIFIER_N64_CONTROLLER

#define STR_EQ(a, b) (strcmp(a, b) == 0)

#endif // DEFINES_H_
