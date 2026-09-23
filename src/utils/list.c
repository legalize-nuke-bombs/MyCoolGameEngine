//
// Created by nikita on 22.09.2026.
//

#include "list.h"

#include <stdlib.h>

struct list {
    void **data;
    int capacity;
    int count;
};

struct list* list_create(int capacity) {
    if (capacity <= 0) {
        capacity = 1;
    }
    struct list *list = malloc(sizeof(struct list));
    list->data = malloc(sizeof(void *) * capacity);
    list->capacity = capacity;
    list->count = 0;
    return list;
}
void list_destroy(struct list *list) {
    free(list->data);
    free(list);
}

int list_count(const struct list *list) {
    return list->count;
}

void* list_get(const struct list *list, const int index) {
    return list->data[index];
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
void list_add(struct list *list, void *data) {
    if (list->count >= list->capacity) {
        list_realloc(list);
    }
    list->data[list->count++] = data;
}