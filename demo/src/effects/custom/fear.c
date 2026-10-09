//
// Created by nikita on 09.10.2026.
//

#include "fear.h"

#include "../effect_internal.h"

struct fear {
    struct effect base;
};

static void fear_on_refresh(struct effect *base, const struct effect *incoming);

const struct effect_vtable fear_vtable = {
    .key = EFFECT_FEAR_KEY,
    .size = sizeof(struct fear),
    .on_refresh = fear_on_refresh
};

static void fear_on_refresh(struct effect *base, const struct effect *incoming) {
    base->duration += incoming->duration;
}
