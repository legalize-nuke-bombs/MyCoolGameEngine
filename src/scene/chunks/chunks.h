//
// Created by nikita on 29.09.2026.
//

#ifndef MYCOOLGAMEENGINE_CHUNKS_H
#define MYCOOLGAMEENGINE_CHUNKS_H

#include "../../utils/rect.h"

struct chunks;
struct component;
struct dictionary;

struct chunks *chunks_create();
void chunks_destroy(struct chunks *this);

void chunks_clear(struct chunks *this);

void chunks_get_rect_indexes(const struct chunks *this, struct rect rect, int *x_start, int *x_end, int *y_start, int *y_end);

void chunks_register_component(struct chunks *this, struct component *component);

struct dictionary* chunks_chunk_get_types(const struct chunks *this, int index_x, int index_y);
struct dictionary* chunks_chunk_get_components_by_type(const struct chunks *this, int index_x, int index_y, const char *component_type);

#endif //MYCOOLGAMEENGINE_CHUNKS_H
