//
// Created by nikita on 03.10.2026.
//

#include "skilled.h"



#include <stdlib.h>

#include "skill.h"
#include "skill_factory.h"
#include "../../../devices/keyboard.h"
#include "../../../factories/factories.h"
#include "../../../scene/entity.h"
#include "../../../scene/scene.h"
#include "../../../scene/components/component_internal.h"
#include "../../../utils/action.h"
#include "../../../utils/parser.h"

#define SKILLS_NUM 1

struct skilled {
    struct component base;

    struct skill* skills[SKILLS_NUM];
    struct action* hotkey_actions[SKILLS_NUM];
    unsigned int hotkey_tokens[SKILLS_NUM];
};

static struct component* skilled_clone(struct component base, const struct component *component);
static void skilled_destroy(struct component *base);
static void skilled_awake(struct component *base);
static void skilled_disable(struct component *base);
static void skilled_update(struct component *base, const struct update_context *context);

static const struct component_vtable skilled_vtable = {
    .component_key = skilled_component_key,
    .on_clone = skilled_clone,
    .on_destroy = skilled_destroy,
    .on_awake = skilled_awake,
    .on_disable = skilled_disable,
    .on_update = skilled_update
};

const char* skilled_component_key(void) {
    return "skilled";
}

// TODO с зависимостями беда беда
struct component* skilled_create(struct parser *parser, struct entity *parent) {
    struct skilled *this = calloc(1, sizeof(struct skilled));
    struct component *base = (struct component *) this;
    component_base_create(base, &skilled_vtable, parent);
    const struct skill_factory* skill_factory = (struct skill_factory*)factories_get("skill_factory");
    this->skills[0] = skill_factory_produce(skill_factory, parser_next(parser), parser, parent);
    return base;
}

static struct component* skilled_clone(struct component base, const struct component *component) {
    const struct skilled *skilled = (const struct skilled *) component;

    struct skilled* this = calloc(1, sizeof(struct skilled));
    this->base = base;
    // TODO skill cloning
    return (struct component*)this;
}

static void skilled_destroy(struct component *base) {
    const struct skilled *this = (struct skilled *) base;
    for (int i = 0; i < SKILLS_NUM; i++) {
        if (this->skills[i]) {
            skill_destroy(this->skills[i]);
        }
    }
}

static void skilled_invoke_skill(void *listener, void *context) {
    const struct skilled *this = listener;
    if (this->skills[0] == NULL) {
        return;
    }
    skill_invoke(this->skills[0]);
}

static void skilled_awake(struct component *base) {
    struct skilled *this = (struct skilled *) base;
    for (int i = 0; i < SKILLS_NUM; i++) {
        if (this->skills[i]) {
            skill_enable(this->skills[i]);
        }
    }
    this->hotkey_actions[0] = keyboard_require_action_on_key_pressed("1");
    action_subscribe(this->hotkey_actions[0], this, skilled_invoke_skill, &this->hotkey_tokens[0]);
}

static void skilled_disable(struct component *base) {
    const struct skilled *this = (struct skilled *) base;
    action_unsubscribe(this->hotkey_actions[0], this->hotkey_tokens[0]);
}

static void skilled_update(struct component *base, const struct update_context *context) {
    const struct skilled *this = (struct skilled *) base;
    for (int i = 0; i < SKILLS_NUM; i++) {
        if (this->skills[i]) {
            skill_update(this->skills[i], context->dt);
        }
    }
}