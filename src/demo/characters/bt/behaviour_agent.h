//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BEHAVIOUR_AGENT_H
#define MYCOOLGAMEENGINE_BEHAVIOUR_AGENT_H

struct entity;
struct component_vtable;

const char* behaviour_agent_component_key(void);

extern const struct component_vtable behaviour_agent_vtable;

#endif //MYCOOLGAMEENGINE_BEHAVIOUR_AGENT_H
