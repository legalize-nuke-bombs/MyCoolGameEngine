#include "collision_response.h"

#include <stddef.h>
#include <string.h>

bool collision_response_try_parse(const char *name, enum collision_response *response) {
    if (name == NULL) {
        return false;
    }
    if (strcmp(name, "ignore") == 0) {
        *response = collision_response_ignore;
        return true;
    }
    if (strcmp(name, "overlap") == 0) {
        *response = collision_response_overlap;
        return true;
    }
    if (strcmp(name, "block") == 0) {
        *response = collision_response_block;
        return true;
    }
    return false;
}
