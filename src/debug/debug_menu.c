#include "debug_menu.h"

#include "../font/font.h"
#include "../player/player.h"
#include "../util/macros.h"

#include <display.h>
#include <n64sys.h>
#include <rdpq.h>
#include <rdpq_font.h>
#include <rdpq_text.h>
#include <stdbool.h>

bool state = true;

heap_stats_t stats;

void debug_menu_render(float delta_time, float elapsed) {
	if (!state) return;

	rdpq_textparms_t name_params = {
		.width = 30,
		.height = (short) display_get_height(),
		.align = ALIGN_RIGHT,
		.valign = VALIGN_TOP,
	};

	rdpq_textparms_t value_params = {
		.width = (short) display_get_width(),
		.height = (short) display_get_height(),
		.align = ALIGN_LEFT,
		.valign = VALIGN_TOP,
	};

	sys_get_heap_stats(&stats);

	rdpq_sync_pipe();

	rdpq_text_printf(&name_params, FONT_NORMAL, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 0, "fps:");
	rdpq_text_printf(&value_params, FONT_NORMAL, OVERSCAN_PAD_X + 35, OVERSCAN_PAD_Y + 0, "%.1f", display_get_fps());

	rdpq_text_printf(&name_params, FONT_NORMAL, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 10, "time:");
	rdpq_text_printf(&value_params, FONT_NORMAL, OVERSCAN_PAD_X + 35, OVERSCAN_PAD_Y + 10, "%.2f s", elapsed);

	rdpq_text_printf(&name_params, FONT_NORMAL, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 20, "delta:");
	rdpq_text_printf(&value_params, FONT_NORMAL, OVERSCAN_PAD_X + 35, OVERSCAN_PAD_Y + 20, "%.9f s", delta_time);

	rdpq_text_printf(&name_params, FONT_NORMAL, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 30, "mem:");
	rdpq_text_printf(&value_params, FONT_NORMAL, OVERSCAN_PAD_X + 35, OVERSCAN_PAD_Y + 30, "%d / %d B", stats.used, stats.total);

	rdpq_text_printf(&name_params, FONT_NORMAL, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 40, "xpos:");
	rdpq_text_printf(&value_params, FONT_NORMAL, OVERSCAN_PAD_X + 35, OVERSCAN_PAD_Y + 40, "%.2f", player.transform.position.x);

	rdpq_text_printf(&name_params, FONT_NORMAL, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 50, "ypos:");
	rdpq_text_printf(&value_params, FONT_NORMAL, OVERSCAN_PAD_X + 35, OVERSCAN_PAD_Y + 50, "%.2f", player.transform.position.y);

	rdpq_text_printf(&name_params, FONT_NORMAL, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 60, "zpos:");
	rdpq_text_printf(&value_params, FONT_NORMAL, OVERSCAN_PAD_X + 35, OVERSCAN_PAD_Y + 60, "%.2f", player.transform.position.z);

	rdpq_text_printf(&name_params, FONT_NORMAL, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 70, "yaw:");
	rdpq_text_printf(&value_params, FONT_NORMAL, OVERSCAN_PAD_X + 35, OVERSCAN_PAD_Y + 70, "%f", player.camera_transform.rotation.y);

	rdpq_text_printf(&name_params, FONT_NORMAL, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 80, "pitch:");
	rdpq_text_printf(&value_params, FONT_NORMAL, OVERSCAN_PAD_X + 35, OVERSCAN_PAD_Y + 80, "%f", player.camera_transform.rotation.x);

}

void debug_menu_toggle() {
	state = !state;
}
