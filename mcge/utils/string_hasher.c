//
// Created by nikita on 22.09.2026.
//

#include "string_hasher.h"

int string_hasher_djb2(const char *string) {
    unsigned long hash = 5381;
    int c;

    while ((c = (unsigned char)*string++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return (int)hash;
}