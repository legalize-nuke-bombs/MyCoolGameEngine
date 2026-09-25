//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_H
#define MYCOOLGAMEENGINE_ENGINE_H

#include "engine_arguments.h"

struct engine* engine_create();
void engine_destroy(struct engine *this);

void engine_execute(struct engine *this, struct engine_arguments args);

#endif //MYCOOLGAMEENGINE_ENGINE_H
