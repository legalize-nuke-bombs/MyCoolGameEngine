//
// Created by nikita on 01.10.2026.
//

#include "interpreter_scene_physics.h"
#include "../../interpreter_command_parent.h"
#include "interpreter_scene_physics_new_layer.h"
#include "interpreter_scene_physics_new_material.h"


struct interpreter_command* interpreter_scene_physics_create() {
    struct interpreter_command_parent* this = interpreter_command_parent_create("physics", 3);
    interpreter_command_parent_capture_child(this, interpreter_scene_physics_new_layer_create());
    interpreter_command_parent_capture_child(this, interpreter_scene_physics_new_material_create());
    return interpreter_command_parent_as_interpreter_command(this);
}
