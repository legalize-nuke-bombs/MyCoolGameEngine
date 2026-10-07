#include "random.h"

#include <stdint.h>
#include <stdio.h>

#include "../msystems/msystem.h"
#include "mcge/logging/logger.h"

#if defined(_WIN32) || defined(_WIN64)
    #define WIN_RANDOM
#else
    #define UNIX_RANDOM
#endif


#if defined WIN_RANDOM
    #include <windows.h>
    #include <bcrypt.h>
#endif


static struct {
    #if defined UNIX_RANDOM
        FILE *unix_random_device;
    #endif
} random;


static void random_create() {
    #if defined UNIX_RANDOM
        random.unix_random_device = fopen("/dev/urandom", "rb");
    #endif
}

static void random_destroy() {
    #if defined UNIX_RANDOM
        if (random.unix_random_device) fclose(random.unix_random_device);
    #endif
}

const struct msystem random_msystem = {
    .name = "random",
    .on_create = random_create,
    .on_destroy = random_destroy
};


static void random_random_bytes(void *buffer, unsigned int len) {
    #if defined WIN_RANDOM
        const NTSTATUS status = BCryptGenRandom(
            NULL,
            buffer,
            len,
            BCRYPT_USE_SYSTEM_PREFERRED_RNG
        );
        if (status != 0) {
            logger_error("BCryptGenRandom failed with exit code %ld", status);
        }
    #else
        if (random.unix_random_device == NULL) {
            logger_error("Failed to access unix random device");
            return;
        }
        unsigned int read = 0;
        while (read != len) {
            read += fread((uint8_t*)buffer + read, 1, len - read, random.unix_random_device);
        }

    #endif
}

int random_next_int(const int l, const int r) {
    if (l >= r) {
        return l;
    }
    uint32_t entropy;
    random_random_bytes(&entropy, sizeof(entropy));
    return l + (entropy % (r - l));
}

MCGE_API uint128_t random_next_uint128() {
    uint128_t entropy;
    random_random_bytes(&entropy, sizeof(entropy));
    return entropy;
}

double random_next_double(const double l, const double r) {
    if (l >= r) {
        return l;
    }
    uint32_t entropy;
    random_random_bytes(&entropy, sizeof(entropy));
    const double scale = (double)entropy / UINT32_MAX;
    return l + scale * (r - l);
}
