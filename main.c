#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "src/engine/engine.h"
#include "src/engine/engine_arguments.h"
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
    logger_init(logger_level_extract(argc, argv));
    const struct engine_arguments engine_arguments = {
        .script_path = script_path_extract(argc, argv),
        .dev_mode = dev_mode_extract(argc, argv)
    };

    struct engine *engine = engine_create();
    engine_execute(engine, engine_arguments);
    engine_destroy(engine);

    return 0;
}
