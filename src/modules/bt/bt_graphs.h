#ifndef MYCOOLGAMEENGINE_BT_GRAPHS_H
#define MYCOOLGAMEENGINE_BT_GRAPHS_H

struct bt_graph;
struct asset_type;

extern const struct asset_type bt_graphs_asset_type;

struct bt_graph* bt_graphs_get(const char *name);

#endif //MYCOOLGAMEENGINE_BT_GRAPHS_H
