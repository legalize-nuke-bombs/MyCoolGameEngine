#include "src/core/entity.h"

int main(void) {
    struct entity firstEntity;
    entity_init(&firstEntity);
    entity_print(&firstEntity);
    return 0;
}
