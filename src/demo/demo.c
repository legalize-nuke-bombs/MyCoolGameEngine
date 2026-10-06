#include "demo.h"

#include "../modules/bt/bt_node_factory.h"
#include "../scene/components/component_factory.h"
#include "characters/bt/behaviour_agent.h"
#include "characters/bt/is_scared.h"
#include "characters/bt/player_chase.h"
#include "characters/bt/player_is_near.h"
#include "characters/bt/player_run_away.h"
#include "characters/effects.h"
#include "characters/mana.h"
#include "characters/player.h"
#include "characters/skills/custom/fear_balls/fear_ball.h"
#include "characters/skills/skilled.h"
#include "enviornment/clock.h"
#include "enviornment/sky.h"
#include "scenes/game_closer.h"
#include "scenes/scene_switcher.h"
#include "special/pulsator.h"
#include "world/forest.h"

void demo_install(void) {
    component_factory_register(pulsator_component_key(), pulsator_create);
    component_factory_register(forest_component_key(), forest_create);
    component_factory_register(scene_switcher_component_key(), scene_switcher_create);
    component_factory_register(game_closer_component_key(), game_closer_create);
    component_factory_register(clock_component_key(), clock_create);
    component_factory_register(sky_component_key(), sky_create);
    component_factory_register(player_component_key(), player_create);
    component_factory_register(behaviour_agent_component_key(), behaviour_agent_create);
    component_factory_register(mana_component_key(), mana_create);
    component_factory_register(skilled_component_key(), skilled_create);
    component_factory_register(fear_ball_component_key(), fear_ball_create);
    component_factory_register(effects_component_key(), effects_create);

    bt_node_factory_register(BT_NODE_PLAYER_IS_NEAR, player_is_near_parse);
    bt_node_factory_register(BT_NODE_PLAYER_CHASE, player_chase_parse);
    bt_node_factory_register(BT_NODE_PLAYER_RUN_AWAY, player_run_away_parse);
    bt_node_factory_register(BT_NODE_IS_SCARED, is_scared_parse);
}
