//
// Created by nikita on 15.10.2026.
//

#include "allocator.h"
#include <stdlib.h>
#include <string.h>
#include "mcge/engine/msystem.h"
#include "mcge/logging/logger.h"


static struct {
    ull total_allocations_num;
    ull total_allocations_size;

    ull total_frees_num;
    ull total_frees_size;
} allocator;



static ull allocator_ptr_size(void *ptr) {
    return _msize(ptr);
}


static void allocator_create(void) {
    memset(&allocator, 0, sizeof(allocator));
}

static void allocator_destroy(void) {
    if (allocator_get_active_allocations_num() == 0 && allocator_get_active_allocations_size() == 0) {
        logger_info("Allocator finished with no leaks detected. Total allocations: %llu (%llu bytes)", allocator.total_allocations_num, allocator.total_allocations_size);
    }
    else {
        logger_error("Allocator finished with leaks detected. Total allocations: %llu (%llu bytes). Total frees: %llu (%llu bytes)", allocator.total_allocations_num, allocator.total_allocations_size, allocator.total_frees_num, allocator.total_frees_size);
    }
    memset(&allocator, 0, sizeof(allocator));
}


const struct msystem allocator_msystem = {
    .name = "allocator",
    .on_create = allocator_create,
    .on_destroy = allocator_destroy
};


void* allocator_calloc(const unsigned int elements, const unsigned int element_size) {
    void *ptr = calloc(elements, element_size);
    allocator.total_allocations_num++;
    allocator.total_allocations_size += allocator_ptr_size(ptr);
    return ptr;
}
void allocator_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    allocator.total_frees_num++;
    allocator.total_frees_size += allocator_ptr_size(ptr);
    free(ptr);
}


ull allocator_get_total_allocations_num(void) {
    return allocator.total_allocations_num;
}
ull allocator_get_total_allocations_size(void) {
    return allocator.total_allocations_size;
}

ull allocator_get_total_frees_num(void) {
    return allocator.total_frees_num;
}
ull allocator_get_total_frees_size(void) {
    return allocator.total_frees_size;
}

ull allocator_get_active_allocations_num(void) {
    return allocator.total_allocations_num - allocator.total_frees_num;
}
ull allocator_get_active_allocations_size(void) {
    return allocator.total_allocations_size - allocator.total_frees_size;
}