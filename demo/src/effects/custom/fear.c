//
// Created by nikita on 09.10.2026.
//

#include "fear.h"

#include "../effect_internal.h"

struct fear {
    struct effect base;
};



static struct color fear_get_color() {
    return color_red_light;
}

static void fear_on_refresh(struct effect *base, const struct effect *incoming) {
    base->duration += incoming->duration;
}


const struct effect_vtable fear_vtable = {
    .key = EFFECT_FEAR_KEY,
    .size = sizeof(struct fear),
    .get_color = fear_get_color,
    .on_refresh = fear_on_refresh
};

