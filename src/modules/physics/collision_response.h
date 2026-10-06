#ifndef MYCOOLGAMEENGINE_COLLISION_RESPONSE_H
#define MYCOOLGAMEENGINE_COLLISION_RESPONSE_H

#include <stdbool.h>

// From the weakest to the strongest: a pair of layers without a rule takes the weaker of their two defaults
enum collision_response {
    collision_response_ignore,
    collision_response_overlap,
    collision_response_block
};

bool collision_response_try_parse(const char *name, enum collision_response *response);

#endif //MYCOOLGAMEENGINE_COLLISION_RESPONSE_H
