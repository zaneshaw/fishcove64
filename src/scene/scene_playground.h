#ifndef SCENE_PLAYGROUND_H_
#define SCENE_PLAYGROUND_H_

#include "scene.h"

extern scene_t scene_playground;

void scene_playground_load(scene_t* this);
void scene_playground_update(scene_t* this, float delta_time, float elapsed);
void scene_playground_render(scene_t* this);

#endif // SCENE_PLAYGROUND_H_
