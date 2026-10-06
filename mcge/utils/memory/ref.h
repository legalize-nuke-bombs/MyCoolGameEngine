//
// Created by Nikita on 06.10.2026.
//

#ifndef MYCOOLGAMEENGINE_REF_H
#define MYCOOLGAMEENGINE_REF_H

#include "control_block.h"
#include "../../api.h"

struct ref {
    struct control_block *_block;
};

typedef struct ref ref;

MCGE_API void ref_destroy(ref *this);

MCGE_API ref ref_copy(const ref *ref);

MCGE_API void* ref_ptr(const ref *this);

#endif //MYCOOLGAMEENGINE_REF_H
