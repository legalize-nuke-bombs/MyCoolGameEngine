//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_FACTORIES_H
#define MYCOOLGAMEENGINE_FACTORIES_H

struct msystem;
struct factory;

extern const struct msystem factories_msystem;

struct factory* factories_get(const char *key);

#endif //MYCOOLGAMEENGINE_FACTORIES_H
