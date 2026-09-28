#include "random.h"

#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "../subsystems/subsystem_internal.h"


static const char* random_get_name() {
    return "random";
}

static void random_on_enable(struct subsystem* subsystem, struct engine_arguments arguments);

static struct subsystem_vtable engine_lifecycle_vtable = {
    .name = random_get_name,
    .on_destroy = NULL,
    .on_enable = random_on_enable,
    .on_disable = NULL
};


struct random {
    struct subsystem base;
};


struct subsystem* random_create(const struct subsystem_collection* collections) {
    struct random* this = calloc(1, sizeof(struct random));
    struct subsystem* base = (struct subsystem*)this;
    subsystem_create(base, &engine_lifecycle_vtable, collections);
    return base;
}

static void random_on_enable(struct subsystem* subsystem, struct engine_arguments arguments) {
    srand((unsigned int)time(NULL));
}

int random_next_int(struct random* random, const int l, const int r) {
    if (l >= r) {
        return l;
    }
    return l + rand() % (r - l + 1);
}

double random_next_double(struct random* random, const double l, const double r) {
    if (l >= r) {
        return l;
    }
    const double scale = (double)rand() / (double)RAND_MAX;
    return l + scale * (r - l);
}
