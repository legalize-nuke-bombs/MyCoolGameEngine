//
// Created by nikita on 22.09.2026.
//

#include "list.h"

#include <stdlib.h>

void list_init(struct list *list, int capacity) {
    if (capacity <= 0) {
        capacity = 1;
    }
    list->_data = malloc(sizeof(void *) * capacity);
    list->_capacity = capacity;
    list->_count = 0;
}
void list_destroy(const struct list *list) {
    free(list->_data);
}

int list_count(const struct list *list) {
    return list->_count;
}

void* list_get(const struct list *list, const int index) {
    return list->_data[index];
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
void list_add(struct list *list, void *data) {
    if (list->_count >= list->_capacity) {
        list_realloc(list);
    }
    list->_data[list->_count++] = data;
}