//
// Created by Nikita on 08.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BOW_H
#define MYCOOLGAMEENGINE_BOW_H

#include "bower_stats.h"

struct bow;

const char* bow_component_key(void);

extern const struct component_vtable bow_vtable;

void bow_set_stats(struct bow* this, struct bower_stats stats);

struct character* bow_try_find_target(struct bow* this);

#endif //MYCOOLGAMEENGINE_BOW_H
