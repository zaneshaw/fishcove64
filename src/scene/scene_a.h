#ifndef SCENE_A_H_
#define SCENE_A_H_

#include "scene.h"

extern scene_t scene_a;

void scene_a_load(scene_t* this);
void scene_a_update(scene_t* this, float delta_time, float elapsed);
void scene_a_render(scene_t* this);

#endif // SCENE_A_H_
