#include <stddef.h>

#include "src/engine/engine.h"

int main(void) {
    struct engine *engine = engine_create();
    if (engine == NULL) {
        return 1;
    }

    engine_run(engine);

    engine_destroy(engine);

    return 0;
}
