//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_RESTARTER_H
#define MYCOOLGAMEENGINE_ENGINE_RESTARTER_H

struct subsystem_collection;
struct engine_restarter;

struct subsystem* engine_restarter_create(const struct subsystem_collection *subsystems);

#endif //MYCOOLGAMEENGINE_ENGINE_RESTARTER_H
