//
// Created by Nikita on 25.09.2026.
//

#include "file_listener.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


struct file_listener {
    char* path;
    time_t last_modified;
};

static time_t get_file_modification_time(const char* path) {
    struct stat attr;
    if (stat(path, &attr) == 0) {
        return attr.st_mtime;
    }
    return 0;
}

struct file_listener* file_listener_create(const char* path) {
    struct file_listener* this = malloc(sizeof(struct file_listener));

    this->path = strdup(path);
    this->last_modified = get_file_modification_time(path);

    return this;
}

void file_listener_destroy(struct file_listener *this) {
    free(this->path);
    free(this);
}

bool file_listener_is_file_changed(struct file_listener *this) {
    const time_t current_modified = get_file_modification_time(this->path);

    if (current_modified != this->last_modified) {
        this->last_modified = current_modified;
        return true;
    }

    return false;
}
