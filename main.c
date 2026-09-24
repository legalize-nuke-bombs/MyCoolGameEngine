#include <stddef.h>
#include <stdlib.h>

#include "src/engine/engine.h"
#include "src/logging/logger.h"
#include "version.h"

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


int main(const int argc, char *argv[]) {
    const char* script_path = script_path_extract(argc, argv);
    const int logger_level = logger_level_extract(argc, argv);

    logger_init(logger_level);
    logger_info("MyCoolGameEngine v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

    struct engine *engine = engine_create();
    if (engine == NULL) {
        return 1;
    }

    engine_execute(engine, script_path);

    engine_destroy(engine);

    return 0;
}
