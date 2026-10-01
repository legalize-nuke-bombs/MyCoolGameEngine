#include "interpreter_catalog.h"
#include "../interpreter_command_parent.h"
#include "interpreter_catalog_add.h"


struct interpreter_command* interpreter_catalog_create() {
    struct interpreter_command_parent* this = interpreter_command_parent_create("catalog", 3);
    interpreter_command_parent_capture_child(this, interpreter_catalog_add_create());
    return interpreter_command_parent_as_interpreter_command(this);
}
