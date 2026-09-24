//
// Created by nikita on 24.09.2026.
//

#include "interpreter_window.h"

#include "interpreter_window_new.h"
#include "../interpreter_command_parent.h"


struct interpreter_command* interpreter_window_create() {
    struct interpreter_command_parent* this = interpreter_command_parent_create("window", 3);
    interpreter_command_parent_capture_child(this, interpreter_window_new_create());
    return interpreter_command_parent_as_interpreter_command(this);
}