//
// Created by nikita on 03.10.2026.
//

#include "printer.h"

#include <stdlib.h>

#include "../skill_internal.h"
#include "../../../../utils/parser.h"
#include "../../../../utils/system_message_box.h"

struct printer {
    struct skill base;
    char *title;
    char *body;
};

static void printer_on_destroy(struct skill *base);
static bool printer_invoke(struct skill* base);

static const struct skill_vtable printer_vtable = {
    .key = SKILL_PRINTER_KEY,
    .on_destroy = printer_on_destroy,
    .on_invoke = printer_invoke
};


struct skill* printer_parse(struct parser *parser, struct entity *self) {
    struct printer* this = calloc(1, sizeof(struct printer));
    struct skill* base = (struct skill*)this;
    skill_base_parse(base, &printer_vtable, parser, self);
    this->title = parser_next_dup(parser);
    this->body = parser_next_dup(parser);
    return base;
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