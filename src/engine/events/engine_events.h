//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_EVENTS_H
#define MYCOOLGAMEENGINE_ENGINE_EVENTS_H

struct engine_events;
struct subsystem_collection;

struct subsystem* engine_events_create(const struct subsystem_collection *subsystems);

struct action* engine_events_pre_frame(struct engine_events *this);
struct action* engine_events_pre_physics(struct engine_events *this);
struct action* engine_events_on_physics(struct engine_events *this);
struct action* engine_events_post_physics(struct engine_events *this);
struct action* engine_events_pre_rendering(struct engine_events *this);
struct action* engine_events_on_rendering(struct engine_events *this);
struct action* engine_events_post_rendering(struct engine_events *this);
struct action* engine_events_on_native_event(struct engine_events *this);


#endif //MYCOOLGAMEENGINE_ENGINE_EVENTS_H
