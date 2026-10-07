#ifndef MYCOOLGAMEENGINE_COLLISION_RESPONSE_H
#define MYCOOLGAMEENGINE_COLLISION_RESPONSE_H

#include <stdbool.h>
#include "../../api.h"

enum collision_response {
    collision_response_ignore,
    collision_response_overlap,
    collision_response_block
};

MCGE_API bool collision_response_try_parse(const char *name, enum collision_response *response);

#endif //MYCOOLGAMEENGINE_COLLISION_RESPONSE_H
