//
// Created by Nikita on 08.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BOW_H
#define MYCOOLGAMEENGINE_BOW_H

struct bow;

const char* bow_component_key(void);

extern const struct component_vtable bow_vtable;

struct character* bow_try_find_target(struct bow* this);

#endif //MYCOOLGAMEENGINE_BOW_H
