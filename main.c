#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "src/engine/engine.h"
#include "src/engine/engine_arguments.h"
#include "src/logging/logger.h"


static void parse_arguments(const int argc, char* argv[], struct engine_arguments *out_args, int *out_logger_level) {
    out_args->data_root = NULL;
    out_args->script_path = NULL;
    out_args->dev_mode = false;
    *out_logger_level = LOGGER_LEVEL_INFO;

    if (argc > 1) out_args->data_root = argv[1];
    if (argc > 2) out_args->script_path = argv[2];
    if (argc > 3) {
        *out_logger_level = (int)strtol(argv[3], NULL, 10);
    }
    if (argc > 4) {
        out_args->dev_mode = (strcmp(argv[4], "1") == 0 || strcmp(argv[4], "true") == 0);
    }
}
int main(const int argc, char *argv[]) {
    int logger_level;
    struct engine_arguments engine_arguments;

    parse_arguments(argc, argv, &engine_arguments, &logger_level);

    logger_init(logger_level);

    struct engine *engine = engine_create();
    engine_execute(engine, engine_arguments);
    engine_destroy(engine);

    return 0;
}
