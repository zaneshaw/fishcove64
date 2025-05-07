#ifndef INTERACTION_H_
#define INTERACTION_H_

#include <stdbool.h>

typedef struct interaction_s interaction_t;
typedef struct interaction_s {
	bool enabled;

	char label[16];
	void (*interact)(interaction_t* this);
} interaction_t;

#endif // INTERACTION_H_
