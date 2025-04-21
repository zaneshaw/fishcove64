#include "debug_menu.h"

#include "../util/macros.h"
#include "../player/player.h"

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

	rdpq_textparms_t debug_text_params = {
		.width = (short) display_get_width(),
		.height = (short) display_get_height(),
		.align = ALIGN_LEFT,
		.valign = VALIGN_TOP,
	};

	sys_get_heap_stats(&stats);

	rdpq_sync_pipe();
	rdpq_text_printf(&debug_text_params, FONT_BUILTIN_DEBUG_MONO, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 0, "  fps: %.1f", display_get_fps());
	rdpq_text_printf(&debug_text_params, FONT_BUILTIN_DEBUG_MONO, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 10, " time: %.2f s", elapsed);
	rdpq_text_printf(&debug_text_params, FONT_BUILTIN_DEBUG_MONO, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 20, "delta: %.9f s", delta_time);
	rdpq_text_printf(&debug_text_params, FONT_BUILTIN_DEBUG_MONO, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 30, "  mem: %d / %d B", stats.used, stats.total);
	rdpq_text_printf(&debug_text_params, FONT_BUILTIN_DEBUG_MONO, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 40, " xpos: %f", player.transform.position.x);
	rdpq_text_printf(&debug_text_params, FONT_BUILTIN_DEBUG_MONO, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 50, " zpos: %f", player.transform.position.z);
	rdpq_text_printf(&debug_text_params, FONT_BUILTIN_DEBUG_MONO, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 60, "  yaw: %f", player.camera_transform.rotation.y);
	rdpq_text_printf(&debug_text_params, FONT_BUILTIN_DEBUG_MONO, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + 70, "pitch: %f", player.camera_transform.rotation.x);
}

void debug_menu_toggle() {
	state = !state;
}
