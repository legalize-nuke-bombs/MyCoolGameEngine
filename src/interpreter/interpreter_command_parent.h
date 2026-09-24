//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_COMMAND_PARENT_H
#define MYCOOLGAMEENGINE_INTERPRETER_COMMAND_PARENT_H

struct interpreter_command_parent;
struct interpreter_command;

struct interpreter_command_parent* interpreter_command_parent_create(const char* name, int dim);

void interpreter_command_parent_capture_child(const struct interpreter_command_parent* this, struct interpreter_command* child);

struct interpreter_command* interpreter_command_parent_as_interpreter_command(struct interpreter_command_parent* this);

#endif //MYCOOLGAMEENGINE_INTERPRETER_COMMAND_PARENT_H
