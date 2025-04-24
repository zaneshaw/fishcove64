#ifndef LERP_H_
#define LERP_H_

float lerp(float a, float b, float t, float good_enough) {
	if (fabsf(a - b) <= good_enough)
		return b;

	return a + (b - a) * t;
}

#endif // LERP_H_
