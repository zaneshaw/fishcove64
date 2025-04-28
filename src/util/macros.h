#ifndef DEFINES_H_
#define DEFINES_H_

#include <joypad.h>

#define OVERSCAN_PAD_X (int) 20
#define OVERSCAN_PAD_Y (int) 15

#define JOYPAD JOYPAD_PORT_1
#define JOYPAD_IS_READY joypad_is_connected(JOYPAD) && joypad_get_identifier(JOYPAD) == JOYBUS_IDENTIFIER_N64_CONTROLLER

#define str_eq(a, b) (strcmp(a, b) == 0)

#endif // DEFINES_H_
