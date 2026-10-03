//
// Created by nikita on 03.10.2026.
//

#include "behaviour_agent.h"

#include <stdlib.h>
#include <string.h>

#include "../../../catalogs/catalogs.h"
#include "../../../logging/logger.h"
#include "../../../modules/bt/bt_node.h"
#include "../../../modules/bt/bt_graph.h"
#include "../../../scene/entity.h"
#include "../../../scene/scene.h"
#include "../../../scene/components/component_internal.h"
#include "../../../subsystems/subsystem_collection.h"
#include "../../../utils/parser.h"
#include "../../../demo/bt/blackboard.h"


struct behaviour_agent {
    struct component base;

    char* bt_graph_name;
    struct bt_graph *bt_graph;
};

const char* behaviour_agent_component_key(void) {
    return "behaviour_agent";
}
static struct component* behaviour_agent_clone(struct component base, const struct component *component);
static void behaviour_agent_awake(struct component *base);
static void behaviour_agent_on_simulation_chunk_update(struct component *base, const struct update_context *context);
static void behaviour_agent_on_destroy(struct component *base);

static const struct component_vtable behaviour_agent_vtable = {
    .component_key = behaviour_agent_component_key,
    .on_clone = behaviour_agent_clone,
    .on_awake = behaviour_agent_awake,
    .on_simulation_chunk_update = behaviour_agent_on_simulation_chunk_update,
    .on_destroy = behaviour_agent_on_destroy
};


struct component* behaviour_agent_create(struct parser *parser, struct entity *parent) {
    struct behaviour_agent *this = calloc(1, sizeof(struct behaviour_agent));
    struct component *base = (struct component *) this;
    component_base_create(base, &behaviour_agent_vtable, parent);
    this->bt_graph_name = parser_next_dup(parser);
    return base;
}

static struct component* behaviour_agent_clone(struct component base, const struct component *component) {
    const struct behaviour_agent *behaviour_agent = (const struct behaviour_agent *) component;

    struct behaviour_agent* this = calloc(1, sizeof(struct behaviour_agent));
    this->base = base;
    this->bt_graph_name = strdup(behaviour_agent->bt_graph_name);
    return (struct component*)this;
}

static void behaviour_agent_on_destroy(struct component *base) {
    const struct behaviour_agent *this = (struct behaviour_agent*)base;
    if (this->bt_graph_name) free(this->bt_graph_name);
}

static void behaviour_agent_awake(struct component *base) {
    struct behaviour_agent *this = (struct behaviour_agent*)base;
    if (this->bt_graph_name) {
        this->bt_graph = catalogs_get_item((struct catalogs*)subsystem_collection_get(scene_get_subsystems(component_get_scene(base)), "catalogs"), "bt_graph", this->bt_graph_name);
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
