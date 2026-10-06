//
// Created by nikita on 03.10.2026.
//

#include "skilled.h"



#include <stdlib.h>

#include "skill.h"
#include "skill_factory.h"
#include "../../../devices/keyboard.h"
#include "../../../logging/logger.h"
#include "../../../scene/entity.h"
#include "../../../scene/scene.h"
#include "../../../scene/components/component_internal.h"
#include "../../../utils/action.h"
#include "../../../utils/fields.h"

#define SKILLS_NUM 9

// The skill in the slot number i is invoked by the key number i
static const char *const skilled_hotkeys[SKILLS_NUM] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

struct skilled_slot {
    struct skill* skill;
    struct action* hotkey_action;
    unsigned int hotkey_token;
};

struct skilled {
    struct component base;

    struct skilled_slot slots[SKILLS_NUM];
};

static void skilled_on_create(struct component *base, struct fields *fields);
static void skilled_destroy(struct component *base);
static void skilled_awake(struct component *base);
static void skilled_disable(struct component *base);
static void skilled_update(struct component *base, const struct update_context *context);

const struct component_vtable skilled_vtable = {
    .component_key = skilled_component_key,
    .size = sizeof(struct skilled),
    .on_create = skilled_on_create,
    .on_destroy = skilled_destroy,
    .on_awake = skilled_awake,
    .on_disable = skilled_disable,
    .on_update = skilled_update
};

const char* skilled_component_key(void) {
    return "skilled";
}

static void skilled_on_create(struct component *base, struct fields *fields) {
    struct skilled *this = (struct skilled *) base;

    const struct fields_list *skills = fields_get_list(fields, "skills");
    if (fields_list_count(skills) > SKILLS_NUM) {
        logger_warn("Skilled expected at most %d skills, got %d", SKILLS_NUM, fields_list_count(skills));
    }
    for (int i = 0; i < fields_list_count(skills) && i < SKILLS_NUM; i++) {
        this->slots[i].skill = skill_factory_produce(fields_list_get(skills, i), component_get_parent(base));
    }
}

static void skilled_destroy(struct component *base) {
    const struct skilled *this = (struct skilled *) base;
    for (int i = 0; i < SKILLS_NUM; i++) {
        if (this->slots[i].skill) {
            skill_destroy(this->slots[i].skill);
        }
    }
}

static void skilled_invoke_skill(void *listener, void *context) {
    const struct skilled_slot *slot = listener;
    skill_invoke(slot->skill);
}

static void skilled_awake(struct component *base) {
    struct skilled *this = (struct skilled *) base;
    for (int i = 0; i < SKILLS_NUM; i++) {
        struct skilled_slot *slot = &this->slots[i];
        if (slot->skill == NULL) {
            continue;
        }
        skill_enable(slot->skill);
        slot->hotkey_action = keyboard_require_action_on_key_pressed(skilled_hotkeys[i]);
        action_subscribe(slot->hotkey_action, slot, skilled_invoke_skill, &slot->hotkey_token);
    }
}

static void skilled_disable(struct component *base) {
    struct skilled *this = (struct skilled *) base;
    for (int i = 0; i < SKILLS_NUM; i++) {
        struct skilled_slot *slot = &this->slots[i];
        if (slot->hotkey_action == NULL) {
            continue;
        }
        action_unsubscribe(slot->hotkey_action, slot->hotkey_token);
        slot->hotkey_action = NULL;
    }
}

static void skilled_update(struct component *base, const struct update_context *context) {
    const struct skilled *this = (struct skilled *) base;
    for (int i = 0; i < SKILLS_NUM; i++) {
        if (this->slots[i].skill) {
            skill_update(this->slots[i].skill, context->dt);
        }
    }
}