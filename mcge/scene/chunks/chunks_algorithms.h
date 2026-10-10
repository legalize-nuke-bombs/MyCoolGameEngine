//
// Created by nikita on 10.10.2026.
//

#ifndef MYCOOLGAMEENGINE_CHUNKS_ALGORITHMS_H
#define MYCOOLGAMEENGINE_CHUNKS_ALGORITHMS_H

#include "../../utils/rect.h"
#include "../../api.h"

struct component;

MCGE_API void chunks_algorithms_for_each(struct rect rect, bool type_validation(const struct component *target), void f(struct component *target, void *context), void *f_context);

MCGE_API void chunks_algorithms_for_each_typed(struct rect rect, const char *component_key, void f(struct component *target, void *context), void *f_context);

#endif //MYCOOLGAMEENGINE_CHUNKS_ALGORITHMS_H
