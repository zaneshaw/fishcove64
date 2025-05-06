#include "debug_draw.h"

#include "../player/player.h"
#include "../util/constants.h"

#include <GL/gl.h>
#include <GL/gl_integration.h>
#include <GL/glu.h>
#include <libdragon.h>
#include <t3d/t3dmath.h>

GLfloat box_vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f
};

GLuint box_indices[24] = {
	0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7
};

void debug_draw_init() {
	float aspect_ratio = (float) display_get_width() / (float) display_get_height();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, aspect_ratio, GL_NEAR_PLANE, GL_FAR_PLANE);
}

void debug_draw_look() {
	transform_t eye = player_get_eye();
	vector3_t eye_forward = {
		fm_cosf(eye.rotation.x) * fm_sinf(eye.rotation.y),
		-fm_sinf(eye.rotation.x),
		fm_cosf(eye.rotation.x) * fm_cosf(eye.rotation.y),
	};
	vector3_t eye_target;
	vector3_add(&eye_target, &eye.position, &eye_forward);

	gl_context_begin();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		eye.position.x, eye.position.y, eye.position.z, eye_target.x, eye_target.y, eye_target.z, 0, 1, 0
	);

	gl_context_end();
}

void debug_draw_box_gl(box_t* box, fm_vec3_t* color) {
	vector3_t* p = &box->pos;
	vector3_t* e = &box->half_extents;

	glPushMatrix();
	T3DMat4 mat;
	t3d_mat4_from_srt(
		&mat,
		(fm_vec3_t) { { e->x * 2, e->y * 2, e->z * 2 } }.v,
		box->quat.v,
		(fm_vec3_t) { { p->x, p->y, p->z } }.v
	);

	float matt[4 * 4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matt[i * 4 + j] = mat.m[i][j];
		}
	}

	glMultMatrixf(matt);

	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(color->x, color->y, color->z);
	glVertexPointer(3, GL_FLOAT, 0, box_vertices);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, box_indices);

	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

void debug_draw_box(box_t* box, fm_vec3_t* color) {
	gl_context_begin();

	rdpq_sync_pipe();
	debug_draw_box_gl(box, color);
	gl_context_end();
}

void debug_draw_cylinder(cylinder_t* cylinder) {
}
