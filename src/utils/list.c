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
        .data = NULL,
        .capacity = capacity,
        .count = 0
    };
    return list;
}
void list_destroy(struct list *this) {
    free(this->data);
}

int list_count(const struct list *this) {
    return this->count;
}

void* list_get(const struct list *this, const int index) {
    return this->data[index];
}

static void list_realloc(struct list *list) {
    const int new_capacity = 2 * list->capacity;
    void **new_data = malloc(sizeof(void *) * new_capacity);
    for (int i = 0; i < list->count; i++) {
        new_data[i] = list->data[i];
    }
    free(list->data);
    list->data = new_data;
    list->capacity = new_capacity;
}
void list_add(struct list *this, void *data) {
    if (this->data == NULL) {
        this->data = malloc(sizeof(void *) * this->capacity);
    }
    else if (this->count >= this->capacity) {
        list_realloc(this);
    }
    this->data[this->count++] = data;
}
void list_set(const struct list *this, int index, void *data) {
    this->data[index] = data;
}

void list_swap(const struct list *this, const int index1, const int index2) {
    void* tmp = this->data[index1];
    this->data[index1] = this->data[index2];
    this->data[index2] = tmp;
}

void list_remove_nulls(struct list *this) {
    int kept_count = 0;
    for (int i = 0; i < this->count; i++) {
        if (this->data[i] != NULL) {
            this->data[kept_count++] = this->data[i];
        }
    }
    this->count = kept_count;
}

void list_clear(struct list *this) {
    this->count = 0;
}

void list_pop_back(struct list *this) {
    this->count--;
}