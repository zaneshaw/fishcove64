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

void vector3_cross(vector3_t* out, vector3_t* a, vector3_t* b) {
	out->x = a->y * b->z - a->z * b->y;
	out->y = a->z * b->x - a->x * b->z;
	out->z = a->x * b->y - a->y * b->x;
}

void vector3_rotate_by_quat(vector3_t* out, vector3_t* v, fm_quat_t* q) {
	vector3_t q_vec = { q->x, q->y, q->z };

	vector3_t cross_qv_v;
	vector3_cross(&cross_qv_v, &q_vec, v);

	vector3_t term2;
	vector3_scale(&term2, &cross_qv_v, q->w * 2.0f);

	vector3_t cross_qv_cross_qv_v;
	vector3_cross(&cross_qv_cross_qv_v, &q_vec, &cross_qv_v);

	vector3_t term3;
	vector3_scale(&term3, &cross_qv_cross_qv_v, 2.0f);

	vector3_t temp;
	vector3_add(&temp, &term2, &term3);

	vector3_t rotated_v;
	vector3_add(&rotated_v, v, &temp);

	*out = rotated_v;
}
