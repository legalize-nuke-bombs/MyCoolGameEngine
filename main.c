#include "src/core/entity.h"

int main(void) {
    struct entity firstEntity;
    init_entity(&firstEntity);
    print_entity(&firstEntity);
    return 0;
}
