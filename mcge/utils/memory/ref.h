//
// Created by Nikita on 06.10.2026.
//

#ifndef MYCOOLGAMEENGINE_REF_H
#define MYCOOLGAMEENGINE_REF_H

#include "control_block.h"

struct ref {
    struct control_block *_block;
};

typedef struct ref ref;

void ref_destroy(ref *this);

ref ref_copy(const ref *ref);

void* ref_ptr(const ref *this);

#endif //MYCOOLGAMEENGINE_REF_H
