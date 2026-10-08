//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SKILL_H
#define MYCOOLGAMEENGINE_SKILL_H

struct skill;
struct entity;

void skill_destroy(struct skill *this);

void skill_enable(struct skill *this);

void skill_update(struct skill *this, double dt);

double skill_get_cooldown_left(const struct skill *this);
double skill_get_cooldown_full(const struct skill *this);

enum skill_invoke_result {
    skill_invoke_scheduled,
    skill_invoke_insufficient_mana,
    skill_invoke_cooldown,
    skill_invoke_runtime_error,
    skill_invoke_hands_are_busy
};
enum skill_invoke_result skill_schedule_invoke(struct skill *this);

#endif //MYCOOLGAMEENGINE_SKILL_H
