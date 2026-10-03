//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_FACTORIES_H
#define MYCOOLGAMEENGINE_FACTORIES_H

struct subsystem;
struct subsystem_collection;
struct factories;
struct factory;

struct subsystem* factories_create(const struct subsystem_collection *subsystems);

struct factory* factories_get(const struct factories *this, const char *key);

#endif //MYCOOLGAMEENGINE_FACTORIES_H
