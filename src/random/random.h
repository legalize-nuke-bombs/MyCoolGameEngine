//
// Created by nikita on 28.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RANDOM_H
#define MYCOOLGAMEENGINE_RANDOM_H

struct subsystem_collection;
struct random;

struct subsystem* random_create(const struct subsystem_collection* subsystems);

int random_next_int(struct random* random, int l, int r);
double random_next_double(struct random* random, double l, double r);

#endif //MYCOOLGAMEENGINE_RANDOM_H
