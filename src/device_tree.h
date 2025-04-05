//
// Created by Bryan Keller on 3/31/25.
//

#ifndef DEVICE_TREE_H
#define DEVICE_TREE_H

#include "types.h"

u32 device_tree_start;
u32 device_tree_end;

void build_device_tree();
void print_device_tree(void *device_tree_ptr);

#endif //DEVICE_TREE_H
