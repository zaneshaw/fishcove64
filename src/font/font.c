#include "font.h"

#include <rdpq_font.h>
#include <rdpq_text.h>

void font_init() {
	// normal
	rdpq_font_t* font_enter_command = rdpq_font_load("rom:/fonts/EnterCommand.font64");
	rdpq_text_register_font(1, font_enter_command);

	// bold
	rdpq_font_t* font_enter_command_bold = rdpq_font_load("rom:/fonts/EnterCommand-Bold.font64");
	rdpq_text_register_font(2, font_enter_command_bold);

	// italic
	rdpq_font_t* font_enter_command_italic = rdpq_font_load("rom:/fonts/EnterCommand-Italic.font64");
	rdpq_text_register_font(3, font_enter_command_italic);
}
