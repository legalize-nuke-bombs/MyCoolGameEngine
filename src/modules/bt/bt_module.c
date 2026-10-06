#include "bt_module.h"

#include "bt_node_factory.h"
#include "../../msystems/msystem.h"

const struct msystem bt_msystem = {
    .name = "bt",
    .on_create = bt_node_factory_create,
    .on_destroy = bt_node_factory_destroy
};
