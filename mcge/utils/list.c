//
// Created by nikita on 22.09.2026.
//

#include "list.h"

#include <stdlib.h>

struct list list_create(int capacity) {
    if (capacity <= 0) {
        capacity = 1;
    }
    const struct list list = {
        ._data = NULL,
        ._capacity = capacity,
        ._count = 0
    };
    return list;
}
void list_destroy(struct list *this) {
    free(this->_data);
}

int list_count(const struct list *this) {
    return this->_count;
}

void* list_get(const struct list *this, const int index) {
    return this->_data[index];
}

static void list_realloc(struct list *list) {
    const int new_capacity = 2 * list->_capacity;
    void **new_data = malloc(sizeof(void *) * new_capacity);
    for (int i = 0; i < list->_count; i++) {
        new_data[i] = list->_data[i];
    }
    free(list->_data);
    list->_data = new_data;
    list->_capacity = new_capacity;
}
void list_add(struct list *this, void *data) {
    if (this->_data == NULL) {
        if (this->_capacity < 1) {
            this->_capacity = 1;
        }
        this->_data = malloc(sizeof(void *) * this->_capacity);
    }
    else if (this->_count >= this->_capacity) {
        list_realloc(this);
    }
    this->_data[this->_count++] = data;
}
void list_set(const struct list *this, int index, void *data) {
    this->_data[index] = data;
}

void list_swap(const struct list *this, const int index1, const int index2) {
    void* tmp = this->_data[index1];
    this->_data[index1] = this->_data[index2];
    this->_data[index2] = tmp;
}

void list_remove_nulls(struct list *this) {
    int kept_count = 0;
    for (int i = 0; i < this->_count; i++) {
        if (this->_data[i] != NULL) {
            this->_data[kept_count++] = this->_data[i];
        }
    }
    this->_count = kept_count;
}

void list_clear(struct list *this) {
    this->_count = 0;
}

void list_pop_back(struct list *this) {
    this->_count--;
}