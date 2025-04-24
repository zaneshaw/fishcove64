#ifndef LERP_H_
#define LERP_H_

float lerp(float a, float b, float t, float close_enough) {
	if (fabsf(a - b) <= close_enough)
		return b;

	return a + (b - a) * t;
}

#endif // LERP_H_
