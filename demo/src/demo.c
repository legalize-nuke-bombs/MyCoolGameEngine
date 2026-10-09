#include "demo.h"

#include <mcge/mcge.h>

#include "character/character.h"
#include "health/destroy_on_death.h"
#include "bt/behaviour_agent.h"
#include "bt/is_scared.h"
#include "bt/player_chase.h"
#include "bt/player_is_near.h"
#include "bt/player_run_away.h"
#include "effects/effects.h"
#include "health/health.h"
#include "health/health_bar.h"
#include "mana/mana.h"
#include "character/player.h"
#include "bow/arrow.h"
#include "bow/arrow_damager.h"
#include "bow/bow.h"
#include "bow/bowman.h"
#include "hands/hands.h"
#include "hands/hands_bar.h"
#include "movement/keyboard_movement.h"
#include "movement/movement.h"
#include "skills/custom/fear_balls/fear_ball.h"
#include "skills/skilled.h"
#include "environment/clock.h"
#include "environment/sky.h"
#include "scenes/game_closer.h"
#include "scenes/scene_switcher.h"
#include "world/pulsator.h"
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
    component_factory_register(&destroy_on_death_vtable);
    component_factory_register(&health_bar_vtable);
    component_factory_register(&hands_vtable);
    component_factory_register(&movement_vtable);
    component_factory_register(&keyboard_movement_vtable);
    component_factory_register(&hands_bar_vtable);
    component_factory_register(&bow_vtable);
    component_factory_register(&arrow_vtable);
    component_factory_register(&character_vtable);
    component_factory_register(&arrow_damager_vtable);
    component_factory_register(&bowman_vtable);

    bt_node_factory_register(&player_is_near_vtable);
    bt_node_factory_register(&player_chase_vtable);
    bt_node_factory_register(&player_run_away_vtable);
    bt_node_factory_register(&is_scared_vtable);
}
