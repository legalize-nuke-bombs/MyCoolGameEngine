//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_H
#define MYCOOLGAMEENGINE_ENGINE_H

#include "engine_arguments.h"

struct msystem;

void engine_create(void);
void engine_destroy(void);

void engine_register_msystem(const struct msystem *msystem);

void engine_execute(struct engine_arguments args);

#endif //MYCOOLGAMEENGINE_ENGINE_H
