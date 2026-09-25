//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SUBSYSTEM_COLLECTION_H
#define MYCOOLGAMEENGINE_SUBSYSTEM_COLLECTION_H

struct subsystem_collection;
struct subsystem;

struct subsystem_collection* subsystem_collection_create();
void subsystem_collection_destroy(struct subsystem_collection* this);

void subsystem_collection_enable_all(struct subsystem_collection* this);
void subsystem_collection_disable_all(struct subsystem_collection* this);

struct subsystem* subsystem_collection_get(struct subsystem_collection* this, const char* name);

#endif //MYCOOLGAMEENGINE_SUBSYSTEM_COLLECTION_H
