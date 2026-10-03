//
// Created by nikita on 03.10.2026.
//

#include "skilled.h"



#include <stdlib.h>

#include "skill.h"
#include "../../../../scene/components/component_internal.h"
#include "custom/printer.h"

#define SKILLS_NUM 4

struct skilled {
    struct component base;
    struct skill* skills[SKILLS_NUM];
};

static struct component* skilled_clone(struct component base, const struct component *component);
static void skilled_destroy(struct component *base);

static const struct component_vtable skilled_vtable = {
    .component_key = skilled_component_key,
    .on_clone = skilled_clone,
    .on_destroy = skilled_destroy
};

const char* skilled_component_key(void) {
    return "skilled";
}

static void skilled_setup_skills(struct skilled *this) {
    const struct component* base = (struct component*)this;
    this->skills[0] = printer_create(component_get_parent(base));
}

struct component* skilled_create(struct parser *parser, struct entity *parent) {
    struct skilled *this = calloc(1, sizeof(struct skilled));
    struct component *base = (struct component *) this;
    component_base_create(base, &skilled_vtable, parent);
    skilled_setup_skills(this);
    return base;
}

static struct component* skilled_clone(struct component base, const struct component *component) {
    const struct skilled *skilled = (const struct skilled *) component;

    struct skilled* this = calloc(1, sizeof(struct skilled));
    this->base = base;
    skilled_setup_skills(this);
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