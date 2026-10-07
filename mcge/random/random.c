#include "random.h"

#include <stddef.h>
#include <stdlib.h>

#include "../msystems/msystem.h"


static unsigned long long ctr = 0;

static void random_on_enable(struct engine_arguments arguments) {
    srand(12345);
}

const struct msystem random_msystem = {
    .name = "random",
    .on_enable = random_on_enable
};

int random_next_int(const int l, const int r) {
    if (l >= r) {
        return l;
    }
    ctr++;
    return l + (ctr % (r - l));
}

double random_next_double(const double l, const double r) {
    if (l >= r) {
        return l;
    }
    const double scale = (double)rand() / (double)RAND_MAX;
    return l + scale * (r - l);
}
