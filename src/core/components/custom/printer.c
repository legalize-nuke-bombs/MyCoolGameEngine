#include "printer.h"

#include <stdio.h>

void printer_init(struct component *self, struct entity *parent, const char *string, double interval) {
    component_init(self, parent);
    self->vtable = &printer_vtable;

    struct printer *this = (struct printer *)self;

    this->intervalString = string;
    this->interval = interval;
    this->timer = 0.0;
}

void printer_update(struct component *self, const double dt) {
    struct printer *this = (struct printer *)self;

    if (this->intervalString == NULL) {
        return;
    }

    this->timer += dt;
    if (this->timer >= this->interval) {
        printf("%s\n", this->intervalString);
        this->timer -= this->interval;
    }
}

void printer_destroy(struct component *self) {
    const struct printer *this = (struct printer *)self;

    if (this->lastString == NULL) {
        return;
    }
    printf("%s\n", this->lastString);
}
