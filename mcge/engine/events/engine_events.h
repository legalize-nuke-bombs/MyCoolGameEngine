//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_EVENTS_H
#define MYCOOLGAMEENGINE_ENGINE_EVENTS_H

#include "../../api.h"

struct msystem;
struct action;

MCGE_API extern const struct msystem engine_events_msystem;

MCGE_API struct action* engine_events_pre_frame(void);
MCGE_API struct action* engine_events_pre_physics(void);
MCGE_API struct action* engine_events_on_physics(void);
MCGE_API struct action* engine_events_post_physics(void);
MCGE_API struct action* engine_events_pre_rendering(void);
MCGE_API struct action* engine_events_on_rendering(void);
MCGE_API struct action* engine_events_post_rendering(void);
MCGE_API struct action* engine_events_on_native_event(void);


#endif //MYCOOLGAMEENGINE_ENGINE_EVENTS_H
