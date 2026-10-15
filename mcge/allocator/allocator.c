//
// Created by nikita on 15.10.2026.
//

#include "allocator.h"
#include <stdlib.h>
#include <string.h>
#include "mcge/engine/msystem.h"
#include "mcge/logging/logger.h"


static struct {
    ll total_allocations_num;
    ll total_allocations_size;

    ll total_frees_num;
    ll total_frees_size;
} allocator;



static ll allocator_ptr_size(void *ptr) {
    return _msize(ptr);
}


static void allocator_create(void) {
    memset(&allocator, 0, sizeof(allocator));
}

static void allocator_destroy(void) {
    if (allocator_get_active_allocations_num() == 0 && allocator_get_active_allocations_size() == 0) {
        logger_info("Allocator finished with no leaks detected. Total allocations: %lld (%lld bytes)", allocator.total_allocations_num, allocator.total_allocations_size);
    }
    else {
        logger_error("Allocator finished with leaks detected. Total allocations: %lld (%lld bytes). Total frees: %lld (%lld bytes)", allocator.total_allocations_num, allocator.total_allocations_size, allocator.total_frees_num, allocator.total_frees_size);
    }
    memset(&allocator, 0, sizeof(allocator));
}


const struct msystem allocator_msystem = {
    .name = "allocator",
    .on_create = allocator_create,
    .on_destroy = allocator_destroy
};


void* allocator_new(const unsigned int size) {
    void *ptr = malloc(size);
    allocator.total_allocations_num++;
    allocator.total_allocations_size += allocator_ptr_size(ptr);
    return ptr;
}
void* allocator_new0(const unsigned int elements, const unsigned int element_size) {
    void *ptr = calloc(elements, element_size);
    allocator.total_allocations_num++;
    allocator.total_allocations_size += allocator_ptr_size(ptr);
    return ptr;
}
void allocator_delete(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    allocator.total_frees_num++;
    allocator.total_frees_size += allocator_ptr_size(ptr);
    free(ptr);
}


ll allocator_get_total_allocations_num(void) {
    return allocator.total_allocations_num;
}
ll allocator_get_total_allocations_size(void) {
    return allocator.total_allocations_size;
}

ll allocator_get_total_frees_num(void) {
    return allocator.total_frees_num;
}
ll allocator_get_total_frees_size(void) {
    return allocator.total_frees_size;
}

ll allocator_get_active_allocations_num(void) {
    return allocator.total_allocations_num - allocator.total_frees_num;
}
ll allocator_get_active_allocations_size(void) {
    return allocator.total_allocations_size - allocator.total_frees_size;
}





char* allocator_strcpy(const char *source) {
    if (source == NULL) {
        return NULL;
    }
    const size_t length = strlen(source) + 1;
    char *output = allocator_new(length);
    if (output != NULL) {
        memcpy(output, source, length);
    }
    return output;
}
