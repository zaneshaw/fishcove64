#ifndef COLLISION_H_
#define COLLISION_H_

#include "shapes/box.h"

#include <ccd/ccd.h>

extern ccd_t ccd;
extern box_t** collision_boxes;
extern int collision_count;

void collision_init();
void collision_allocate(int size);
void collision_add(box_t* box);
void collision_free();

#endif // COLLISION_H_
