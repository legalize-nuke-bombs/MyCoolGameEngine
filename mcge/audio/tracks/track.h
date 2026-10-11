//
// Created by nikita on 11.10.2026.
//

#ifndef MYCOOLGAMEENGINE_TRACK_H
#define MYCOOLGAMEENGINE_TRACK_H

#include <stdbool.h>

#include "../../api.h"

struct track {
    unsigned int id;
    struct MIX_Track *native_track;
};

MCGE_API bool track_is_valid(const struct track *this);

#endif //MYCOOLGAMEENGINE_TRACK_H
