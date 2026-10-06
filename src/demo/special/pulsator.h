#ifndef MYCOOLGAMEENGINE_PULSATOR_H
#define MYCOOLGAMEENGINE_PULSATOR_H

struct pulsator;
struct entity;
struct component_vtable;

const char* pulsator_component_key(void);

extern const struct component_vtable pulsator_vtable;

#endif //MYCOOLGAMEENGINE_PULSATOR_H
