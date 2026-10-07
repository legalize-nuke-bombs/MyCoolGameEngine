//
// Created by nikita on 07.10.2026.
//

#include "system_random.h"
#include <windows.h>
#include <bcrypt.h>
#include "../logging/logger.h"

void system_random_random_bytes(void* buffer, const unsigned int len) {
    const NTSTATUS status = BCryptGenRandom(
            NULL,
            buffer,
            len,
            BCRYPT_USE_SYSTEM_PREFERRED_RNG
    );
    if (status != 0) {
        logger_error("BCryptGenRandom failed with exit code %ld", status);
    }
}