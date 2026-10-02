#include "clock.h"

#include <stdlib.h>

#include "../component_internal.h"
#include "../../../utils/parser.h"


static const int DAY_DURATION = 24 * 3600;


struct clock {
    struct component base;

    double seconds;
    double speed;
};

static struct component* clock_clone(struct component base, const struct component *component);
static void clock_update(struct component* base, const struct update_context *context);

static const struct component_vtable clock_vtable = {
    .component_key = clock_component_key,
    .on_clone = clock_clone,
    .on_update = clock_update,
};

const char* clock_component_key(void) {
    return "clock";
}

struct component* clock_create(struct parser *parser, struct entity *parent) {
    struct clock *this = calloc(1, sizeof(struct clock));
    struct component *base = (struct component *) this;
    component_base_create(base, &clock_vtable, parent);
    parser_next_double(parser, &this->seconds);
    parser_next_double(parser, &this->speed);
    return base;
}

static struct component* clock_clone(struct component base, const struct component *component) {
    const struct clock *clock = (struct clock *) component;

    struct clock* this = calloc(1, sizeof(struct clock));
    this->base = base;
    this->seconds = clock->seconds;
    this->speed = clock->speed;
    return (struct component*)this;
}

static void clock_update(struct component* base, const struct update_context *context) {
    struct clock* this = (struct clock*)base;
    this->seconds += this->speed * context->dt;
}

double clock_get_cycle_progress(const struct clock *this) {
    return (double)((int)this->seconds % DAY_DURATION) / (double)DAY_DURATION;
}