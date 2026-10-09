//
// Created by nikita on 09.10.2026.
//

#ifndef MYCOOLGAMEENGINE_EFFECT_H
#define MYCOOLGAMEENGINE_EFFECT_H

#include <stdbool.h>

struct effect;

void effect_destroy(struct effect *this);

void effect_update(struct effect *this, double dt);

const char* effect_key(const struct effect *this);

void effect_mark_destroyed(struct effect *this);
bool effect_is_alive(const struct effect *this);

bool effect_try_refresh(struct effect *this, const struct effect *incoming);

double effect_get_time_left(const struct effect *this);
double effect_get_time_full(const struct effect *this);

#endif //MYCOOLGAMEENGINE_EFFECT_H
