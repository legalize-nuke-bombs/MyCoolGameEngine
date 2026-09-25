//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_LIFECYCLE_H
#define MYCOOLGAMEENGINE_ENGINE_LIFECYCLE_H
#include <stdbool.h>

struct engine_lifecycle;
struct subsystem_collection;

struct subsystem* engine_lifecycle_create(const struct subsystem_collection* collections);

bool engine_lifecycle_is_running(const struct engine_lifecycle *this);
bool engine_lifecycle_restart_required(const struct engine_lifecycle *this);

void engine_lifecycle_mark_stop(struct engine_lifecycle *this);
void engine_lifecycle_mark_restart(struct engine_lifecycle *this);

#endif //MYCOOLGAMEENGINE_ENGINE_LIFECYCLE_H
