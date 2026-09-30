#ifndef MYCOOLGAMEENGINE_PULSATOR_H
#define MYCOOLGAMEENGINE_PULSATOR_H

struct pulsator;
struct entity;
struct parser;

const char* pulsator_component_key(void);

struct component* pulsator_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_PULSATOR_H
