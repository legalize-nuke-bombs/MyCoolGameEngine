#include "demo.h"

#include <mcge/mcge.h>
#include "characters/bt/behaviour_agent.h"
#include "characters/bt/is_scared.h"
#include "characters/bt/player_chase.h"
#include "characters/bt/player_is_near.h"
#include "characters/bt/player_run_away.h"
#include "characters/effects.h"
#include "characters/health.h"
#include "characters/mana.h"
#include "characters/player.h"
#include "skills/custom/fear_balls/fear_ball.h"
#include "skills/skilled.h"
#include "environment/clock.h"
#include "environment/sky.h"
#include "scenes/game_closer.h"
#include "scenes/scene_switcher.h"
#include "special/pulsator.h"
#include "world/forest.h"

void demo_install(void) {
    component_factory_register(&pulsator_vtable);
    component_factory_register(&forest_vtable);
    component_factory_register(&scene_switcher_vtable);
    component_factory_register(&game_closer_vtable);
    component_factory_register(&clock_vtable);
    component_factory_register(&sky_vtable);
    component_factory_register(&player_vtable);
    component_factory_register(&behaviour_agent_vtable);
    component_factory_register(&mana_vtable);
    component_factory_register(&skilled_vtable);
    component_factory_register(&fear_ball_vtable);
    component_factory_register(&effects_vtable);
    component_factory_register(&health_vtable);

    bt_node_factory_register(&player_is_near_vtable);
    bt_node_factory_register(&player_chase_vtable);
    bt_node_factory_register(&player_run_away_vtable);
    bt_node_factory_register(&is_scared_vtable);
}
