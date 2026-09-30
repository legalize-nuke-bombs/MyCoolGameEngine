//
// Created by Nikita on 30.09.2026.
//

#include "chunk.h"

#include <stddef.h>

#include "../../utils/dictionary.h"
#include "../../utils/pointer_dictionary.h"
#include "../../utils/string_dictionary.h"
#include "../components/component.h"
#include "../../logging/logger.h"


static void chunk_lazy_alloc(struct chunk *this) {
    if (this->components == NULL) {
        this->components = pointer_dictionary_build(1);
    }
    if (this->types == NULL) {
        this->types = string_dictionary_build(1);
    }
}

static bool chunk_is_allocated(const struct chunk *this) {
    return this->components;
}


void chunk_destroy(struct chunk *this) {
    if (this->components) {
        dictionary_destroy(this->components);
        this->components = NULL;
    }
    if (this->types) {
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
}

void chunk_try_add_component(struct chunk *this, struct component* component) {
    chunk_lazy_alloc(this);
    if (dictionary_try_add(this->components, component, component)) {
        const char* component_type = component_get_key(component);

        if (dictionary_absent(this->types, (void*)component_type)) {
            struct dictionary* typed_components = pointer_dictionary_build(1);
            dictionary_try_add(this->types, (void*)component_type, typed_components);
        }

        struct dictionary* typed_components = dictionary_get(this->types, (void*)component_type);
        if (!dictionary_try_add(typed_components, component, component)) {
            logger_error("Chunk invariant error. `components` accepted component but typed_components didn't");
        }
    }
}
void chunk_try_remove_component(const struct chunk *this, struct component* component) {
    if (!chunk_is_allocated(this)) {
        return;
    }
    if (dictionary_remove(this->components, component)) {
        const char* component_type = component_get_key(component);

        struct dictionary* typed_components = dictionary_get(this->types, (void*)component_type);
        if (typed_components == NULL) {
            logger_error("Chunk invariant error. Chunk contains component but does not contains `typed_components` for this component");
        }
        else {
            if (!dictionary_remove(typed_components, component)) {
                logger_error("Chunk invariant error. Chunk contains component but does not contains it in `typed_components`");
            }
        }
    }
}

struct dictionary* chunk_get_components(const struct chunk* this) {
    if (!chunk_is_allocated(this)) {
        return NULL;
    }
    return this->components;
}
struct dictionary* chunk_get_components_by_type(const struct chunk* this, const char* component_type) {
    if (!chunk_is_allocated(this)) {
        return NULL;
    }
    return dictionary_get(this->types, (void*)component_type);
}