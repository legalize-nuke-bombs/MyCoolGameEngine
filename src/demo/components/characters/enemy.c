//
// Created by nikita on 03.10.2026.
//

#include "enemy.h"

#include <stdlib.h>

#include "../../../logging/logger.h"
#include "../../../modules/bt/bt_node.h"
#include "../../../scene/components/component_internal.h"
#include "../../../modules/bt/bt_sequence.h"
#include "../../bt/player_is_near.h"
#include "../../bt/blackboard.h"
#include "../../bt/player_chase.h"


static struct bt_node* enemy_build_bt_graph() {
    struct bt_sequence* bt_graph = bt_sequence_create();
    bt_sequence_capture_node(bt_graph, player_is_near_create(10));
    bt_sequence_capture_node(bt_graph, player_chase_create(6.25));
    return (struct bt_node*)bt_graph;
}

static struct bt_node* bt_graph = NULL;
static int bt_graph_ref_counter = 0; /* Crazy technology */

static struct bt_node *enemy_reserve_bt_graph() {
    if (bt_graph == NULL) {
        logger_debug("Enemy behaviour tree graph is building...");
        bt_graph = enemy_build_bt_graph();
    }
    bt_graph_ref_counter++;
    return bt_graph;
}

static void enemy_release_bt_graph() {
    bt_graph_ref_counter--;
    if (bt_graph_ref_counter == 0 && bt_graph != NULL) {
        logger_debug("Enemy behaviour tree graph is destroying...");
        bt_node_destroy(bt_graph);
        bt_graph = NULL;
    }
}


struct enemy {
    struct component base;

    struct bt_node *bt_graph;
};

static struct component* enemy_clone(struct component base, const struct component *component);
static void enemy_on_simulation_chunk_update(struct component *base, const struct update_context *context);
static void enemy_on_destroy(struct component *base);

static const struct component_vtable enemy_vtable = {
    .component_key = enemy_component_key,
    .on_clone = enemy_clone,
    .on_simulation_chunk_update = enemy_on_simulation_chunk_update,
    .on_destroy = enemy_on_destroy
};

const char* enemy_component_key(void) {
    return "enemy";
}

struct component* enemy_create(struct parser *parser, struct entity *parent) {
    struct enemy *this = calloc(1, sizeof(struct enemy));
    struct component *base = (struct component *) this;
    component_base_create(base, &enemy_vtable, parent);
    this->bt_graph = enemy_reserve_bt_graph();
    return base;
}

static struct component* enemy_clone(struct component base, const struct component *component) {
    const struct enemy *enemy = (const struct enemy *) component;

    struct enemy* this = calloc(1, sizeof(struct enemy));
    this->base = base;
    this->bt_graph = enemy_reserve_bt_graph();
    return (struct component*)this;
}

static void enemy_on_destroy(struct component *base) {
    struct enemy *this = (struct enemy*)base;
    enemy_release_bt_graph();
}

static void enemy_on_simulation_chunk_update(struct component *base, const struct update_context *context) {
    const struct enemy *this = (struct enemy*)base;
    struct blackboard bb = {
        .self = component_get_parent(base)
    };
    bt_node_run(this->bt_graph, &bb);
}
