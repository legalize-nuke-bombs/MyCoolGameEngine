#include "component.h"

void component_init(struct component *component, struct entity *parent) {
    component->parent = parent;
}