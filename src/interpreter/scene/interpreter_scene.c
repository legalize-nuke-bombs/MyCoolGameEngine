//
// Created by nikita on 24.09.2026.
//

#include "interpreter_scene.h"
#include "../interpreter_command_parent.h"
#include "interpreter_scene_new_entity.h"


struct interpreter_command* interpreter_scene_create() {
    struct interpreter_command_parent* this = interpreter_command_parent_create("scene", 3);
    interpreter_command_parent_capture_child(this, interpreter_scene_new_entity_create());
    return interpreter_command_parent_as_interpreter_command(this);
}
