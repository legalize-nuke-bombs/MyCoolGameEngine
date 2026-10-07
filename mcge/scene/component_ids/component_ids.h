#ifndef MYCOOLGAMEENGINE_COMPONENT_IDS_H
#define MYCOOLGAMEENGINE_COMPONENT_IDS_H

#include "../../api.h"
#include "../../utils/uint128_t.h"

struct component;
struct component_ids;

MCGE_API struct component_ids* component_ids_create(void);
MCGE_API void component_ids_destroy(struct component_ids *this);

MCGE_API void component_ids_clear(const struct component_ids *this);

MCGE_API struct component* component_ids_try_get(const struct component_ids *this, uint128_t id);

#endif //MYCOOLGAMEENGINE_COMPONENT_IDS_H
