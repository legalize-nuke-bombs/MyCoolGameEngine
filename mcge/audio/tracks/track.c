//
// Created by nikita on 11.10.2026.
//

#include "track.h"

#include <stddef.h>


const struct track track_invalid = {
    .native_track = NULL,
    .id = -1
};


bool track_is_valid(const struct track *this) {
    return this->native_track;
}