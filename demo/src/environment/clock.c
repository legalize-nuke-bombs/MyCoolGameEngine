#include "clock.h"

#include <stdlib.h>

#include <mcge/mcge.h>


static const int DAY_DURATION = 24 * 3600;


struct clock {
    struct component base;

    double seconds;
    double speed;
};

static void clock_on_create(struct component *base, struct fields *fields);
static void clock_update(struct component* base, const struct update_context *context);

const struct component_vtable clock_vtable = {
    .component_key = clock_component_key,
    .size = sizeof(struct clock),
    .on_create = clock_on_create,
    .on_update = clock_update,
};

const char* clock_component_key(void) {
    return "clock";
}

static void clock_on_create(struct component *base, struct fields *fields) {
    struct clock *this = (struct clock *) base;
    this->seconds = fields_get_double(fields, "seconds", 0);
    this->speed = fields_get_double(fields, "speed", 1);
}

static void clock_update(struct component* base, const struct update_context *context) {
    struct clock* this = (struct clock*)base;
    this->seconds += this->speed * context->dt;
    if (this->seconds >= 10 * this->speed) { // TODO remove this shit after the test
        logger_info("Clock is destroying. The process will probably go unstable");
        component_mark_destroyed(base);
    }
}

double clock_get_cycle_progress(const struct clock *this) {
    return (double)((int)this->seconds % DAY_DURATION) / (double)DAY_DURATION;
}