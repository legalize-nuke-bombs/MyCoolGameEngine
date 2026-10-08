//
// Created by Nikita on 08.10.2026.
//

#ifndef MYCOOLGAMEENGINE_ARROW_H
#define MYCOOLGAMEENGINE_ARROW_H

struct arrow;
struct character;

const char* arrow_component_key(void);

extern const struct component_vtable arrow_vtable;

void arrow_launch(struct arrow *this, struct character *target);

#endif //MYCOOLGAMEENGINE_ARROW_H
