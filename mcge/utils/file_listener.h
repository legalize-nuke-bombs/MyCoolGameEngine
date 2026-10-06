//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_FILE_LISTENER_H
#define MYCOOLGAMEENGINE_FILE_LISTENER_H
#include <stdbool.h>
#include "../api.h"

struct file_listener;

MCGE_API struct file_listener* file_listener_create(const char* path);
MCGE_API void file_listener_destroy(struct file_listener *this);

MCGE_API const char* file_listener_get_path(const struct file_listener *this);

MCGE_API bool file_listener_update(struct file_listener *this, double dt);

#endif //MYCOOLGAMEENGINE_FILE_LISTENER_H
