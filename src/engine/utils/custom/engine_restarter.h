//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_RESTARTER_H
#define MYCOOLGAMEENGINE_ENGINE_RESTARTER_H

#include "../../engine_init_arguments.h"

struct engine;
struct engine_restarter;

struct engine_restarter* engine_restarter_create(struct engine *engine, struct engine_init_arguments arguments);
void engine_restarter_destroy(struct engine_restarter *this);

void engine_restarter_awake(struct engine_restarter *this);
void engine_restarter_disable(struct engine_restarter *this);

#endif //MYCOOLGAMEENGINE_ENGINE_RESTARTER_H
