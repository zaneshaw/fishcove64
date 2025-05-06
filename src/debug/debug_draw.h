#ifndef DEBUG_DRAW_H_
#define DEBUG_DRAW_H_

#include "../collision/shapes/box.h"
#include "../collision/shapes/cylinder.h"

typedef enum {
	BOX_DEBUG_NONE,
	BOX_DEBUG_COLLIDE = 1 << 0,
	BOX_DEBUG_INTERACT = 1 << 1,
} box_debug_flags_t;

void debug_draw_init();

void debug_draw_look();
void debug_draw_box(box_t* box, fm_vec3_t* color);
void debug_draw_cylinder(cylinder_t* cylinder);

#endif // DEBUG_DRAW_H_
