//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_UTILS_H
#define MYCOOLGAMEENGINE_ENGINE_UTILS_H

struct engine;
struct engine_utils;

struct engine_utils* engine_utils_create(struct engine *engine);
void engine_utils_destroy(struct engine_utils *this);

void engine_utils_awake(const struct engine_utils *this);
void engine_utils_disable(const struct engine_utils *this);

#endif //MYCOOLGAMEENGINE_ENGINE_UTILS_H
