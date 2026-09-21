#include "src/core/Entity.h"

int main(void) {
    struct Entity firstEntity;
    init_entity(&firstEntity);
    print_entity(&firstEntity);
    return 0;
}
