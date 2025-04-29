#ifndef DEBUG_MENU_H_
#define DEBUG_MENU_H_

#include <time.h>

void debug_menu_init();
void debug_menu_render(float delta_time, float elapsed, time_t now);
void debug_menu_toggle();
void debug_time_toggle();

#endif // DEBUG_MENU_H_
