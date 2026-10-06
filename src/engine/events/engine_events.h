//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_EVENTS_H
#define MYCOOLGAMEENGINE_ENGINE_EVENTS_H

struct msystem;
struct action;

extern const struct msystem engine_events_msystem;

struct action* engine_events_pre_frame(void);
struct action* engine_events_pre_physics(void);
struct action* engine_events_on_physics(void);
struct action* engine_events_post_physics(void);
struct action* engine_events_pre_rendering(void);
struct action* engine_events_on_rendering(void);
struct action* engine_events_post_rendering(void);
struct action* engine_events_on_native_event(void);


#endif //MYCOOLGAMEENGINE_ENGINE_EVENTS_H
