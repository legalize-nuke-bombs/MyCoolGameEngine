//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_PRINT_H
#define MYCOOLGAMEENGINE_INTERPRETER_PRINT_H

struct interpreter_print;
struct interpreter_command;

struct interpreter_print* interpreter_print_create();

struct interpreter_command* interpreter_print_as_interpreter_command(struct interpreter_print* this);

#endif //MYCOOLGAMEENGINE_INTERPRETER_PRINT_H
