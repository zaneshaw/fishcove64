#ifndef GAME_H_
#define GAME_H_

typedef enum {
	GAME_INPUT_NORMAL,
	GAME_INPUT_PAUSE,
} game_input_state_t;

extern game_input_state_t game_input_state;

void pause_render();

#endif // GAME_H_
