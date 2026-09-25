#include <stddef.h>
#include <stdlib.h>

#include "src/engine/engine.h"
#include "src/engine/engine_init_arguments.h"
#include "src/engine/engine_execution_arguments.h"
#include "src/logging/logger.h"


static char* script_path_extract(const int argc, char* argv[]) {
    if (argc >= 2) {
        return argv[1];
    }
    return NULL;
}

int logger_level_extract(const int argc, char* argv[]) {
    if (argc >= 3) {
        return atoi(argv[2]);
    }
    return LOGGER_LEVEL_INFO;
}

bool dev_mode_extract(const int argc, char* argv[]) {
    if (argc >= 4) {
        return atoi(argv[3]);
    }
    return 0;
}


int main(const int argc, char *argv[]) {
    struct engine_init_arguments init_arguments = {
        .logger_level = logger_level_extract(argc, argv),
        .dev_mode = dev_mode_extract(argc, argv)
    };

    struct engine *engine = engine_try_create(init_arguments);
    if (engine == NULL) {
        return 1;
    }

    const struct engine_execution_arguments execution_arguments = {
        .script_path = script_path_extract(argc, argv),
    };

    engine_execute(engine, execution_arguments);

    engine_destroy(engine);

    return 0;
}
