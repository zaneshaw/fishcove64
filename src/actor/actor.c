#include "actor.h"

void actor_draw(actor_t* actor) {
	if (!actor->block) {
		rspq_block_begin();

		t3d_model_draw(actor->model);

		actor->block = rspq_block_end();
	}

	t3d_matrix_push(actor->transform_matrix);

	rspq_block_run(actor->block);

	t3d_matrix_pop(1);
}

void actor_kill(actor_t* actor) {
	t3d_model_free(actor->model);
	free_uncached(actor->transform_matrix);
	if (actor->block) rspq_block_free(actor->block);
	rspq_wait();

	// free collisions
	for (int i = 0; i < actor->collision_count; i++) {
		free(actor->collisions[i].shape);
	}
	free(actor->collisions);

	actor->collision_count = 0;

	free(actor);
}
