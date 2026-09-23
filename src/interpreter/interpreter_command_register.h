//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_COMMAND_REGISTER_H
#define MYCOOLGAMEENGINE_INTERPRETER_COMMAND_REGISTER_H

struct interpreter_command_register;

struct interpreter_command_register* interpreter_command_register_create(const char *name, int capacity);
void interpreter_command_register_destroy(struct interpreter_command_register* this);

struct interpreter_command* interpreter_command_register_try_get_command(const struct interpreter_command_register* this, const char* command_key);

char* interpreter_command_register_alloc_command_list(const struct interpreter_command_register *this);

#endif //MYCOOLGAMEENGINE_INTERPRETER_COMMAND_REGISTER_H
