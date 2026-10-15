//
// Created by nikita on 15.10.2026.
//

#ifndef MYCOOLGAMEENGINE_ALLOCATOR_H
#define MYCOOLGAMEENGINE_ALLOCATOR_H

#include "mcge/api.h"


typedef unsigned long long ull;



extern const struct msystem allocator_msystem;


MCGE_API void* allocator_new(unsigned int size);
MCGE_API void* allocator_new0(unsigned int elements, unsigned int element_size);
MCGE_API void allocator_delete(void *ptr);




MCGE_API ull allocator_get_total_allocations_num(void);
MCGE_API ull allocator_get_total_allocations_size(void);

MCGE_API ull allocator_get_total_frees_num(void);
MCGE_API ull allocator_get_total_frees_size(void);

MCGE_API ull allocator_get_active_allocations_num(void);
MCGE_API ull allocator_get_active_allocations_size(void);

#endif //MYCOOLGAMEENGINE_ALLOCATOR_H
