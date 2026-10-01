//
// Created by Nikita on 01.10.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_ONCE_H
#define MYCOOLGAMEENGINE_INTERPRETER_ONCE_H

struct interpreter_once;
struct interpreter_command;

struct interpreter_once* interpreter_once_create();

struct interpreter_command* interpreter_once_as_interpreter_command(struct interpreter_once* this);

#endif //MYCOOLGAMEENGINE_INTERPRETER_ONCE_H
