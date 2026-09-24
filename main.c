#include <stddef.h>

#include "src/engine/engine.h"
#include "src/logging/logger.h"
#include "version.h"

static char* script_path_extract(const int argc, char* argv[]) {
    char* script_path;
    if (argc >= 2) {
        script_path = argv[1];
    }
    else {
        script_path = NULL;
    }
    return script_path;
}


int main(const int argc, char *argv[]) {
    logger_init(LOGGER_LEVEL_DEBUG);
    logger_info("MyCoolGameEngine v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

    const char* script_path = script_path_extract(argc, argv);

    struct engine *engine = engine_create();
    if (engine == NULL) {
        return 1;
    }

    engine_execute(engine, script_path);

    engine_destroy(engine);

    return 0;
}
