//
// Created by nikita on 11.10.2026.
//

#ifndef MYCOOLGAMEENGINE_TRACK_H
#define MYCOOLGAMEENGINE_TRACK_H

#include <stdint.h>

#include "../../api.h"

struct track {
    unsigned int index;
    uint64_t play_number;
};

MCGE_API extern const struct track track_invalid;

#endif //MYCOOLGAMEENGINE_TRACK_H
