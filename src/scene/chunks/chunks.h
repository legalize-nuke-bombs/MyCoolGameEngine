//
// Created by nikita on 29.09.2026.
//

#ifndef MYCOOLGAMEENGINE_CHUNKS_H
#define MYCOOLGAMEENGINE_CHUNKS_H

struct chunks;
struct component;

struct chunks *chunks_create();
void chunks_destroy(struct chunks *this);

void chunks_clear(const struct chunks *this);

void chunks_register_component(struct chunks *this, struct component *component);

#endif //MYCOOLGAMEENGINE_CHUNKS_H
