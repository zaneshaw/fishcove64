#include "game.h"

#include "../util/constants.h"
#include "../font/font.h"

#include <libdragon.h>

game_input_state_t game_input_state;

void pause_render() {
	rdpq_sync_pipe();
	rdpq_mode_push();

	rdpq_set_mode_standard();
	rdpq_set_prim_color(RGBA32(0x00, 0x00, 0x00, 150));
	rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
	rdpq_fill_rectangle(0, 0, display_get_width(), display_get_height());

	rdpq_textparms_t label_params = {
		.width = (short) display_get_width(),
		.height = (short) display_get_height(),
		.align = ALIGN_LEFT,
		.valign = VALIGN_BOTTOM,
	};

	rdpq_text_printf(&label_params, FONT_NORMAL, OVERSCAN_PAD_X, -OVERSCAN_PAD_Y - 10, "B: save");
	rdpq_text_printf(&label_params, FONT_NORMAL, OVERSCAN_PAD_X, -OVERSCAN_PAD_Y, "A: load");

	rdpq_mode_pop();
}
