#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "src/engine/engine.h"
#include "src/engine/engine_arguments.h"
#include "src/demo/demo.h"
#include "src/logging/logger.h"


static void parse_arguments(const int argc, char* argv[], const char **out_data_root, struct engine_arguments *out_args, int *out_logger_level) {
    *out_data_root = NULL;
    out_args->script_path = NULL;
    out_args->dev_mode = false;
    *out_logger_level = LOGGER_LEVEL_INFO;

    if (argc > 1) *out_data_root = argv[1];
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
    const char *data_root;
    struct engine_arguments engine_arguments;

    parse_arguments(argc, argv, &data_root, &engine_arguments, &logger_level);

    logger_init(logger_level);

    if (data_root != NULL && chdir(data_root) != 0) {
        logger_error("Failed to enter data root %s", data_root);
        return 1;
    }

    engine_create();
    demo_install();
    engine_execute(engine_arguments);
    engine_destroy();

    return 0;
}
