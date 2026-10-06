//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_H
#define MYCOOLGAMEENGINE_ENGINE_H

#include "engine_arguments.h"
#include "../api.h"

struct msystem;

MCGE_API void engine_create(void);
MCGE_API void engine_destroy(void);

MCGE_API void engine_register_msystem(const struct msystem *msystem);

MCGE_API void engine_execute(struct engine_arguments args);

#endif //MYCOOLGAMEENGINE_ENGINE_H
