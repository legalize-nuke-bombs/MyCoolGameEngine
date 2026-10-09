//
// Created by nikita on 09.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BOWER_H
#define MYCOOLGAMEENGINE_BOWER_H

#include "bower_stats.h"

struct bower;

const char* bower_component_key(void);

extern const struct component_vtable bower_vtable;

#endif //MYCOOLGAMEENGINE_BOWER_H
