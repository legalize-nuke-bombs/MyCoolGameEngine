//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_LIST_H
#define MYCOOLGAMEENGINE_LIST_H

#include "../api.h"

struct list {
    void **_data;
    int _capacity;
    int _count;
};

MCGE_API struct list list_create(int capacity);
MCGE_API void list_destroy(struct list *this);

MCGE_API int list_count(const struct list *this);

MCGE_API void* list_get(const struct list *this, int index);

MCGE_API void list_add(struct list *this, void *data);
MCGE_API void list_set(const struct list *this, int index, void *data);

MCGE_API void list_swap(const struct list *this, int index1, int index2);

MCGE_API void list_remove_nulls(struct list *this);

MCGE_API void list_clear(struct list *this);

MCGE_API void list_pop_back(struct list *this);

#endif //MYCOOLGAMEENGINE_LIST_H
