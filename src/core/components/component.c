#include "component.h"

void component_init(struct component *component) {

}

struct entity* component_get_parent(struct component *this) {
    return this->_parent;
}
void component_set_parent(struct component *this, struct entity *parent) {
    this->_parent = parent;
}