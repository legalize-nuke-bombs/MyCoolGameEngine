//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_CLOSER_H
#define MYCOOLGAMEENGINE_ENGINE_CLOSER_H

struct engine;
struct engine_closer;

struct engine_closer* engine_closer_create(struct engine *engine);
void engine_closer_destroy(struct engine_closer *this);

void engine_closer_awake(struct engine_closer *this);
void engine_closer_disable(struct engine_closer *this);

#endif //MYCOOLGAMEENGINE_ENGINE_CLOSER_H
