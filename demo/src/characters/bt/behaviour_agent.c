//
// Created by nikita on 03.10.2026.
//

#include "behaviour_agent.h"

#include <stdlib.h>
#include <string.h>

#include <mcge/mcge.h>
#include "../bt/blackboard.h"


struct behaviour_agent {
    struct component base;

    char* bt_graph_name;
    struct bt_graph *bt_graph;
};

const char* behaviour_agent_component_key(void) {
    return "behaviour_agent";
}
static void behaviour_agent_on_create(struct component *base, struct fields *fields);
static void behaviour_agent_awake(struct component *base);
static void behaviour_agent_on_simulation_chunk_update(struct component *base, const struct update_context *context);
static void behaviour_agent_on_destroy(struct component *base);

const struct component_vtable behaviour_agent_vtable = {
    .component_key = behaviour_agent_component_key,
    .size = sizeof(struct behaviour_agent),
    .on_create = behaviour_agent_on_create,
    .on_awake = behaviour_agent_awake,
    .on_simulation_chunk_update = behaviour_agent_on_simulation_chunk_update,
    .on_destroy = behaviour_agent_on_destroy
};


static void behaviour_agent_on_create(struct component *base, struct fields *fields) {
    struct behaviour_agent *this = (struct behaviour_agent *) base;
    this->bt_graph_name = fields_dup_string(fields, "graph", NULL);
}

static void behaviour_agent_on_destroy(struct component *base) {
    const struct behaviour_agent *this = (struct behaviour_agent*)base;
    if (this->bt_graph_name) free(this->bt_graph_name);
}

static void behaviour_agent_awake(struct component *base) {
    struct behaviour_agent *this = (struct behaviour_agent*)base;
    if (this->bt_graph_name) {
        this->bt_graph = bt_graphs_get(this->bt_graph_name);
        free(this->bt_graph_name);
        this->bt_graph_name = NULL;
    }
    if (this->bt_graph == NULL) {
        logger_warn("Entity %s failed to find behaviour graph, behaviour agent will be destroyed", component_get_global_parent_name(base));
        entity_mark_destroyed(component_get_parent(base));
    }
}

static void behaviour_agent_on_simulation_chunk_update(struct component *base, const struct update_context *context) {
    const struct behaviour_agent *this = (struct behaviour_agent*)base;
    struct blackboard bb = {
        .self = component_get_parent(base)
    };
    bt_node_run(bt_graph_root(this->bt_graph), &bb);
}
