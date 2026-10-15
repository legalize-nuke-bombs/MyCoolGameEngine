//
// Created by nikita on 15.10.2026.
//

#ifndef MYCOOLGAMEENGINE_ALLOCATOR_H
#define MYCOOLGAMEENGINE_ALLOCATOR_H

typedef unsigned long long ull;



extern const struct msystem allocator_msystem;


void* allocator_calloc(unsigned int elements, unsigned int element_size);
void allocator_free(void *ptr);




ull allocator_get_total_allocations_num(void);
ull allocator_get_total_allocations_size(void);

ull allocator_get_total_frees_num(void);
ull allocator_get_total_frees_size(void);

ull allocator_get_active_allocations_num(void);
ull allocator_get_active_allocations_size(void);

#endif //MYCOOLGAMEENGINE_ALLOCATOR_H
