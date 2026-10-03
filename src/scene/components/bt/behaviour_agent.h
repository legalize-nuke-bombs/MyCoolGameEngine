//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BEHAVIOUR_AGENT_H
#define MYCOOLGAMEENGINE_BEHAVIOUR_AGENT_H

struct entity;
struct parser;

const char* behaviour_agent_component_key(void);

struct component* behaviour_agent_create(struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_BEHAVIOUR_AGENT_H
