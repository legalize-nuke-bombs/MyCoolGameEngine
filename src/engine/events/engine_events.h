//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_EVENTS_H
#define MYCOOLGAMEENGINE_ENGINE_EVENTS_H

struct engine_events;

struct engine_events* engine_events_create();
void engine_events_destroy(struct engine_events *this);

struct action* engine_events_pre_frame(const struct engine_events *this);
struct action* engine_events_pre_physics(const struct engine_events *this);
struct action* engine_events_on_physics(const struct engine_events *this);
struct action* engine_events_post_physics(const struct engine_events *this);
struct action* engine_events_pre_rendering(const struct engine_events *this);
struct action* engine_events_on_rendering_phase1(const struct engine_events *this);
struct action* engine_events_on_rendering_phase2(const struct engine_events *this);
struct action* engine_events_on_rendering_phase3(const struct engine_events *this);
struct action* engine_events_post_rendering(const struct engine_events *this);
struct action* engine_events_on_native_event(struct engine_events *this);


#endif //MYCOOLGAMEENGINE_ENGINE_EVENTS_H
