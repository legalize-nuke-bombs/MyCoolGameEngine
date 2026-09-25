//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SUBSYSTEM_COLLECTION_H
#define MYCOOLGAMEENGINE_SUBSYSTEM_COLLECTION_H

struct subsystem_collection;
struct subsystem;
struct engine;

struct subsystem_collection* subsystem_collection_create(struct engine* engine);
void subsystem_collection_destroy(struct subsystem_collection* this);

void subsystem_collection_enable_all(const struct subsystem_collection* this);
void subsystem_collection_disable_all(const struct subsystem_collection* this);

struct subsystem* subsystem_collection_get(const struct subsystem_collection* this, const char* name);

#endif //MYCOOLGAMEENGINE_SUBSYSTEM_COLLECTION_H
