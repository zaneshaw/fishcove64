#include "vector3.h"

vector3_t vector3_right = { 1.0f, 0.0f, 0.0f };
vector3_t vector3_up = { 0.0f, 1.0f, 0.0f };
vector3_t vector3_forward = { 0.0f, 0.0f, 1.0f };
vector3_t vector3_zero = { 0.0f, 0.0f, 0.0f };
vector3_t vector3_one = { 1.0f, 1.0f, 1.0f };

void vector3_add(vector3_t* out, vector3_t* a, vector3_t* b) {
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
}

void vector3_subtract(vector3_t* out, vector3_t* a, vector3_t* b) {
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
}

void vector3_multiply(vector3_t* out, vector3_t* a, vector3_t* b) {
	out->x = a->x * b->x;
	out->y = a->y * b->y;
	out->z = a->z * b->z;
}

void vector3_divide(vector3_t* out, vector3_t* a, vector3_t* b) {
	out->x = a->x / b->x;
	out->y = a->y / b->y;
	out->z = a->z / b->z;
}

void vector3_scale(vector3_t* out, vector3_t* a, float b) {
	out->x = a->x * b;
	out->y = a->y * b;
	out->z = a->z * b;
}
