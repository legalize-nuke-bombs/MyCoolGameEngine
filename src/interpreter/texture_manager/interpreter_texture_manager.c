//
// Created by Nikita on 27.09.2026.
//

#include "interpreter_texture_manager.h"

#include "interpreter_texture_manager_new_texture.h"
#include "../interpreter_command_parent.h"


struct interpreter_command* interpreter_texture_manager_create() {
    struct interpreter_command_parent* this = interpreter_command_parent_create("texture_manager", 3);
    interpreter_command_parent_capture_child(this, interpreter_texture_manager_new_texture_create());
    return interpreter_command_parent_as_interpreter_command(this);
}