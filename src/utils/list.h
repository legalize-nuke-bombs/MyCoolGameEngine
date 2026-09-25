//
// Created by nikita on 22.09.2026.
//

#ifndef MYCOOLGAMEENGINE_LIST_H
#define MYCOOLGAMEENGINE_LIST_H

struct list;

struct list* list_create(int capacity);
void list_destroy(struct list *this);

int list_count(const struct list *this);

void* list_get(const struct list *this, int index);

void list_add(struct list *this, void *data);
void list_set(const struct list *this, int index, void *data);

void list_remove_nulls(struct list *this);

void list_clear(struct list *this);

#endif //MYCOOLGAMEENGINE_LIST_H
