#include "component.h"

#include <stddef.h>

void component_init(struct component *component) {

}
void component_destroy(struct component *component) {
    if (component->_vtable->on_destroy != NULL) {
        component->_vtable->on_destroy(component);
    }
}

struct entity* component_get_parent(struct component *this) {
    return this->_parent;
}
void component_set_parent(struct component *this, struct entity *parent) {
    this->_parent = parent;
}