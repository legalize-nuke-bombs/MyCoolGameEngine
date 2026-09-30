//
// Created by Nikita on 30.09.2026.
//

#include "chunk.h"

#include <stddef.h>

#include "../../utils/dictionary.h"
#include "../../utils/pointer_dictionary.h"
#include "../../utils/string_dictionary.h"
#include "../components/component.h"


static void chunk_lazy_alloc(struct chunk *this) {
    if (this->types == NULL) {
        this->types = string_dictionary_build(1);
    }
}

static bool chunk_is_allocated(const struct chunk *this) {
    return this->types;
}


void chunk_destroy(struct chunk *this) {
    if (!chunk_is_allocated(this)) {
        return;
    }
    struct dictionary_iterator types_iterator = dictionary_begin(this->types);
    struct dictionary_node types_node;
    while (dictionary_next(this->types, &types_iterator, &types_node)) {
        if (types_node.value == NULL) {
            continue;
        }
        dictionary_destroy(types_node.value);
    }
    dictionary_destroy(this->types);
    this->types = NULL;
}
void chunk_clear(const struct chunk *this) {
    if (!chunk_is_allocated(this)) {
        return;
    }
    struct dictionary_iterator types_iterator = dictionary_begin(this->types);
    struct dictionary_node types_node;
    while (dictionary_next(this->types, &types_iterator, &types_node)) {
        if (types_node.value == NULL) {
            continue;
        }
        dictionary_clear(types_node.value);
    }
}

void chunk_try_add_component(struct chunk *this, const struct component* component) {
    chunk_lazy_alloc(this);

    const char* component_type = component_get_key(component);

    struct dictionary* typed_components = dictionary_get(this->types, (void*)component_type);
    if (typed_components == NULL) {
        typed_components = pointer_dictionary_build(1);
        dictionary_try_add(this->types, (void*)component_type, typed_components);
        typed_components = dictionary_get(this->types, (void*)component_type);
    }

    dictionary_try_add(typed_components, (void*)component, (void*)component);
}
void chunk_try_remove_component(const struct chunk *this, const struct component* component) {
    if (!chunk_is_allocated(this)) {
        return;
    }

    const char* component_type = component_get_key(component);

    struct dictionary* typed_components = dictionary_get(this->types, (void*)component_type);
    if (typed_components == NULL) {
        return;
    }

    dictionary_remove(typed_components, (void*)component);
}

struct dictionary* chunk_get_types(const struct chunk* this) {
    return this->types;
}
struct dictionary* chunk_get_components_by_type(const struct chunk* this, const char* component_type) {
    if (!chunk_is_allocated(this)) {
        return NULL;
    }
    return dictionary_get(this->types, (void*)component_type);
}