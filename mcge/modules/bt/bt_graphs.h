#ifndef MYCOOLGAMEENGINE_BT_GRAPHS_H
#define MYCOOLGAMEENGINE_BT_GRAPHS_H

#include "../../api.h"

struct bt_graph;
struct asset_type;

MCGE_API extern const struct asset_type bt_graphs_asset_type;

MCGE_API struct bt_graph* bt_graphs_get(const char *name);

#endif //MYCOOLGAMEENGINE_BT_GRAPHS_H
