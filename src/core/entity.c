#include "entity.h"

#include <stdio.h>
#include <stdlib.h>


#define DEFAULT_COMPONENTS_CAPACITY 1


void entity_init(struct entity *this) {
    this->_name = "Default entity";

    this->_components = malloc(sizeof(struct component*) * DEFAULT_COMPONENTS_CAPACITY);
    this->_components_capacity = DEFAULT_COMPONENTS_CAPACITY;
    this->_components_count = 0;
}
void entity_destroy(const struct entity *this) {
    for (int i = 0; i < this->_components_count; i++) {
        component_destroy(this->_components[i]);
    }
    free(this->_components);
}

const char* entity_get_name(const struct entity *this) {
    return this->_name;
}
void entity_set_name(struct entity *this, const char *name) {
    this->_name = name;
}

int entity_get_components_count(const struct entity *this) {
    return this->_components_count;
}
static void entity_realloc_components(struct entity *this) {
    int new_capacity = 2 * this->_components_capacity;
    struct component **new_components = malloc(sizeof(struct component*) * new_capacity);
    for (int i = 0; i < this->_components_count; i++) {
        new_components[i] = this->_components[i];
    }
    free(this->_components);
    this->_components = new_components;
    this->_components_capacity = new_capacity;
}
void entity_add_component(struct entity *this, struct component *component) {
    if (this->_components_count >= this->_components_capacity) {
        entity_realloc_components(this);
    }
    this->_components[this->_components_count++] = component;

    component_set_parent(component, this);
}

void entity_update(const struct entity *this, const struct update_context *context) {
    for (int i = 0; i < this->_components_count; i++) {
        struct component *component = this->_components[i];
        component_update(component, context);
    }
}

void entity_print(const struct entity *this) {
    printf("Entity %s (%d components count, %d components capacity)\n", this->_name, this->_components_count, this->_components_capacity);
}
