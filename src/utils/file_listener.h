//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_FILE_LISTENER_H
#define MYCOOLGAMEENGINE_FILE_LISTENER_H
#include <stdbool.h>

struct file_listener;

struct file_listener* file_listener_create(const char* path);
void file_listener_destroy(struct file_listener *this);

bool file_listener_update(struct file_listener *this, double dt);

#endif //MYCOOLGAMEENGINE_FILE_LISTENER_H
