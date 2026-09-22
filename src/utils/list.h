//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_LIST_H
#define MYCOOLGAMEENGINE_LIST_H

struct list {
    void **_data;
    int _capacity;
    int _count;
};

void list_init(struct list *list, int capacity);
void list_destroy(const struct list *list);

int list_count(const struct list *list);

void* list_get(const struct list *list, int index);

void list_add(struct list *list, void *data);

#endif //MYCOOLGAMEENGINE_LIST_H
