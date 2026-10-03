//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_FACTORY_H
#define MYCOOLGAMEENGINE_FACTORY_H

struct factory;

void factory_destroy(struct factory *this);

void* factory_produce(struct factory *this, const char *key);

const char* factory_get_key(const struct factory *this);

#endif //MYCOOLGAMEENGINE_FACTORY_H
