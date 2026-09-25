//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SUBSYSTEM_COLLECTION_H
#define MYCOOLGAMEENGINE_SUBSYSTEM_COLLECTION_H

#include "../engine/engine_arguments.h"

struct subsystem_collection;
struct subsystem;
struct engine;

struct subsystem_collection* subsystem_collection_create();
void subsystem_collection_destroy(struct subsystem_collection* this);

void subsystem_collection_enable_all(const struct subsystem_collection* this, struct engine_arguments args);
void subsystem_collection_disable_all(const struct subsystem_collection* this);

struct subsystem* subsystem_collection_get(const struct subsystem_collection* this, const char* name);

#endif //MYCOOLGAMEENGINE_SUBSYSTEM_COLLECTION_H
