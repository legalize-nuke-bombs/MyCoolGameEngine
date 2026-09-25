//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_RESTARTER_H
#define MYCOOLGAMEENGINE_ENGINE_RESTARTER_H

#include "../engine_execution_arguments.h"

struct engine_restarter;
struct engine;

struct engine_restarter* engine_restarter_create(struct engine *engine, struct engine_execution_arguments arguments);
void engine_restarter_destroy(struct engine_restarter *this);

#endif //MYCOOLGAMEENGINE_ENGINE_RESTARTER_H
