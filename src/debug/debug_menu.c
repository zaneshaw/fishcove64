#include "debug_menu.h"

#include "../font/font.h"
#include "../player/player.h"
#include "../util/constants.h"

#include <display.h>
#include <libdragon.h>
#include <n64sys.h>
#include <rdpq.h>
#include <rdpq_font.h>
#include <rdpq_text.h>
#include <stdarg.h>

#define DEBUG_MENU_LINE_HEIGHT 10
#define DEBUG_MENU_VALUE_OFFSET 35

static const char* const MONTH_SHORT_NAMES[12] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

bool menu_state = true;
bool time_state = true;
int cursor = 0;
heap_stats_t stats;

rdpq_textparms_t label_params;
rdpq_textparms_t value_params;
rdpq_textparms_t time_params;

void debug_menu_init() {
	label_params = (rdpq_textparms_t) {
		.width = 30,
		.height = (short) display_get_height(),
		.align = ALIGN_RIGHT,
		.valign = VALIGN_TOP,
	};

	value_params = (rdpq_textparms_t) {
		.width = (short) display_get_width(),
		.height = (short) display_get_height(),
		.align = ALIGN_LEFT,
		.valign = VALIGN_TOP,
	};

	time_params = (rdpq_textparms_t) {
		.width = (short) display_get_width(),
		.height = (short) display_get_height(),
		.align = ALIGN_CENTER,
		.valign = VALIGN_BOTTOM,
	};
}

void debug_menu_draw_entry(const char* label, const char* format, ...) {
	va_list va;
	va_start(va, format);

	rdpq_text_printf(&label_params, FONT_NORMAL, OVERSCAN_PAD_X, OVERSCAN_PAD_Y + cursor * DEBUG_MENU_LINE_HEIGHT, "%s:", label);
	rdpq_text_vprintf(&value_params, FONT_NORMAL, OVERSCAN_PAD_X + DEBUG_MENU_VALUE_OFFSET, OVERSCAN_PAD_Y + cursor * DEBUG_MENU_LINE_HEIGHT, format, va);

	cursor++;
}

void debug_menu_render(float delta_time, float elapsed, time_t now) {
	rdpq_sync_pipe();
	rdpq_mode_push();

	rdpq_set_mode_standard();

	if (menu_state) {
		cursor = 0;

		sys_get_heap_stats(&stats);

		debug_menu_draw_entry("fps", "%.1f", display_get_fps());
		debug_menu_draw_entry("time", "%.2f s", elapsed);
		debug_menu_draw_entry("now", "%d", now);
		debug_menu_draw_entry("delta", "%.9f s", delta_time);
		debug_menu_draw_entry("mem", "%d / %d B", stats.used, stats.total);
		debug_menu_draw_entry("xpos", "%.2f", player.transform.position.x);
		debug_menu_draw_entry("ypos", "%.2f", player.transform.position.y);
		debug_menu_draw_entry("zpos", "%.2f", player.transform.position.z);
		debug_menu_draw_entry("yaw", "%f", player.camera_transform.rotation.y);
		debug_menu_draw_entry("pitch", "%f", player.camera_transform.rotation.x);
	}

	if (time_state) {
		struct tm date_time = *gmtime(&now);
		rdpq_text_printf(
			&time_params,
			FONT_NORMAL,
			0,
			-OVERSCAN_PAD_Y,
			"%d %s %d\n%02d:%02d:%02d %s",
			date_time.tm_mday,
			MONTH_SHORT_NAMES[CLAMP(date_time.tm_mon, 1, 11)],
			1900 + date_time.tm_year,
			date_time.tm_hour == 0 ? 12 : ((date_time.tm_hour - 1) % 12) + 1,
			date_time.tm_min,
			date_time.tm_sec,
			date_time.tm_hour < 12 ? "AM" : "PM"
		);
	}

	rdpq_mode_pop();
}

void debug_menu_toggle() {
	menu_state = !menu_state;
}

void debug_time_toggle() {
	time_state = !time_state;
}
