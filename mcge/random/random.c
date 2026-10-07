#include "random.h"

#include <stddef.h>
#include <stdint.h>

#include "../msystems/msystem.h"
#include "system_random.h"


const struct msystem random_msystem = {
    .name = "random"
};

int random_next_int(const int l, const int r) {
    if (l >= r) {
        return l;
    }
    uint32_t entropy;
    system_random_random_bytes(&entropy, sizeof(entropy));
    return l + (entropy % (r - l));
}

double random_next_double(const double l, const double r) {
    if (l >= r) {
        return l;
    }
    uint32_t entropy;
    system_random_random_bytes(&entropy, sizeof(entropy));
    const double scale = (double)entropy / UINT32_MAX;
    return l + scale * (r - l);
}
