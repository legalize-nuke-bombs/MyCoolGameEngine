//
// Created by Nikita on 06.10.2026.
//

#ifndef MYCOOLGAMEENGINE_HOLDER_H
#define MYCOOLGAMEENGINE_HOLDER_H

#include "control_block.h"

struct holder {
    struct control_block* _block;
};

typedef struct holder holder;

holder holder_create(void *ptr, void (*destructor)(void *ptr));
void holder_destroy(holder *this);

holder holder_copy(const holder *holder);

void* holder_ptr(const holder *this);

#endif //MYCOOLGAMEENGINE_HOLDER_H
