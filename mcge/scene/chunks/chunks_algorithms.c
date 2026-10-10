//
// Created by nikita on 10.10.2026.
//

#include "chunks_algorithms.h"
#include "chunks.h"
#include "../scene.h"
#include "../../utils/dictionary.h"

void chunks_algorithms_for_each(const struct rect rect, bool type_validation(const struct component *target), void f(struct component *target, void *context), void *f_context) {
    const struct chunks *chunks = scene_get_chunks();
    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(chunks, rect, &x_start, &x_end, &y_start, &y_end);
    for (int x = x_start; x <= x_end; x++) {
        for (int y = y_start; y <= y_end; y++) {
            const struct dictionary *types = chunks_chunk_get_types(chunks, x, y);
            if (types == NULL) {
                continue;
            }
            struct dictionary_iterator types_iterator = dictionary_begin(types);
            struct dictionary_node node;
            while (dictionary_next(types, &types_iterator, &node)) {
                const struct dictionary* typed_components = node.value;

                struct dictionary_iterator typed_components_iterator = dictionary_begin(typed_components);
                while (dictionary_next(typed_components, &typed_components_iterator, &node)) {
                    struct component* component = node.value;
                    if (!type_validation(component)) {
                        break;
                    }
                    const struct rect component_rect = component_get_rect(component);
                    if (!rects_intersection(rect, component_rect)) {
                        continue;
                    }
                    f(component, f_context);
                }
            }
        }
    }
}

void chunks_algorithms_for_each_typed(const struct rect rect, const char *component_key, void f(struct component* target, void *context), void *f_context) {
    const struct chunks *chunks = scene_get_chunks();
    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(chunks, rect, &x_start, &x_end, &y_start, &y_end);
    for (int x = x_start; x <= x_end; x++) {
        for (int y = y_start; y <= y_end; y++) {
            const struct dictionary* components = chunks_chunk_get_components_by_type(chunks, x, y, component_key);
            if (components == NULL) {
                continue;
            }

            struct dictionary_iterator iterator = dictionary_begin(components);
            struct dictionary_node node;
            while (dictionary_next(components, &iterator, &node)) {
                struct component* component = node.value;
                const struct rect component_rect = component_get_rect(component);
                if (!rects_intersection(rect, component_rect)) {
                    continue;
                }
                f(component, f_context);
            }
        }
    }
}
