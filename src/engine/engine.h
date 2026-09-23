//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_H
#define MYCOOLGAMEENGINE_ENGINE_H

struct engine* engine_create();
void engine_destroy(struct engine *this);

int engine_run(struct engine *this);

#endif //MYCOOLGAMEENGINE_ENGINE_H
