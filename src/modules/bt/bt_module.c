#include "bt_module.h"

#include "bt_graph.h"
#include "bt_node_factory.h"
#include "../../assets/assets.h"
#include "../../msystems/msystem.h"

static void bt_on_create(void) {
    bt_node_factory_create();
    assets_register(&bt_graph_asset_type);
}

const struct msystem bt_msystem = {
    .name = "bt",
    .on_create = bt_on_create,
    .on_destroy = bt_node_factory_destroy
};
