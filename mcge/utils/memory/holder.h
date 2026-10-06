//
// Created by Nikita on 06.10.2026.
//

#ifndef MYCOOLGAMEENGINE_HOLDER_H
#define MYCOOLGAMEENGINE_HOLDER_H

#include "control_block.h"
#include "ref.h"
#include "../../api.h"

struct holder {
    struct control_block* _block;
};

typedef struct holder holder;

MCGE_API holder holder_create(void *ptr, void (*destructor)(void *ptr));
MCGE_API void holder_destroy(holder *this);

MCGE_API holder holder_copy(const holder *holder);
MCGE_API ref holder_ref(const holder *holder);

MCGE_API void* holder_ptr(const holder *this);

#endif //MYCOOLGAMEENGINE_HOLDER_H
