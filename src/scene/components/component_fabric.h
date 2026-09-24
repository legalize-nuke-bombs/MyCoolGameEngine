//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_COMPONENT_FABRIC_H
#define MYCOOLGAMEENGINE_COMPONENT_FABRIC_H

struct component_fabric;
struct component;
struct parser;
struct entity;

struct component_fabric* component_fabric_create();
void component_fabric_destroy(struct component_fabric *this);

struct component* component_fabric_try_produce_component(struct component_fabric *this, struct parser *parser, struct entity *parent);

#endif //MYCOOLGAMEENGINE_COMPONENT_FABRIC_H
