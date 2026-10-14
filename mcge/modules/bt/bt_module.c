#include "bt_module.h"

#include "bt_graphs.h"
#include "bt_node_factory.h"
#include "../../assets/assets.h"
#include "../../engine/msystem.h"
#include "mcge/logging/logger.h"

static void bt_install(void) {
    logger_info("Bt module is installing...");
    bt_node_factory_create();
    assets_register(&bt_graphs_asset_type);
}

static void bt_on_create(void) {
    bt_install();
}

const struct msystem bt_msystem = {
    .name = "bt",
    .on_create = bt_on_create,
    .on_destroy = bt_node_factory_destroy
};
