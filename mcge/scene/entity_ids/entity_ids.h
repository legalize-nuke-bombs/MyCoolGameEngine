#ifndef MYCOOLGAMEENGINE_ENTITY_IDS_H
#define MYCOOLGAMEENGINE_ENTITY_IDS_H

#include "../../api.h"
#include "../../utils/uint128_t.h"

struct entity;
struct entity_ids;

MCGE_API struct entity_ids* entity_ids_create(void);
MCGE_API void entity_ids_destroy(struct entity_ids *this);

MCGE_API void entity_ids_clear(const struct entity_ids *this);

MCGE_API struct entity* entity_ids_try_get(const struct entity_ids *this, uint128_t id);

#endif //MYCOOLGAMEENGINE_ENTITY_IDS_H
