//
// Created by nikita on 03.10.2026.
//

#include "printer.h"

#include <stdlib.h>

#include "../skill_internal.h"
#include <mcge/mcge.h>

struct printer {
    struct skill base;
    char *title;
    char *body;
};

static void printer_on_create(struct skill *base, struct fields *fields);
static void printer_on_destroy(struct skill *base);
static bool printer_invoke(struct skill* base);

const struct skill_vtable printer_vtable = {
    .key = SKILL_PRINTER_KEY,
    .size = sizeof(struct printer),
    .on_create = printer_on_create,
    .on_destroy = printer_on_destroy,
    .on_invoke = printer_invoke
};


static void printer_on_create(struct skill *base, struct fields *fields) {
    struct printer* this = (struct printer*)base;
    this->title = fields_dup_string(fields, "title", "");
    this->body = fields_dup_string(fields, "body", "");
}

static void printer_on_destroy(struct skill *base) {
    const struct printer* this = (struct printer*)base;
    free(this->title);
    free(this->body);
}

static bool printer_invoke(struct skill* base) {
    const struct printer* this = (struct printer*)base;
    system_message_box_show(this->title, this->body);
    return true;
}