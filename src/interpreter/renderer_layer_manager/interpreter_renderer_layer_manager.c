//
// Created by nikita on 24.09.2026.
//

#include "interpreter_renderer_layer_manager.h"

#include "interpreter_renderer_layer_manager_new_layer.h"
#include "../interpreter_command_parent.h"

struct interpreter_command* interpreter_renderer_layer_manager_create() {
    struct interpreter_command_parent* this = interpreter_command_parent_create("renderer_layer_manager", 3);
    interpreter_command_parent_capture_child(this, interpreter_renderer_layer_manager_new_layer_create());
    return interpreter_command_parent_as_interpreter_command(this);
}