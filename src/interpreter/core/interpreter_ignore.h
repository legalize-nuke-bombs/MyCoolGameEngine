//
// Created by Nikita on 26.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_IGNORE_H
#define MYCOOLGAMEENGINE_INTERPRETER_IGNORE_H

struct interpreter_ignore;
struct interpreter_command;

struct interpreter_ignore* interpreter_ignore_create();

struct interpreter_command* interpreter_ignore_as_interpreter_command(struct interpreter_ignore* this);

#endif //MYCOOLGAMEENGINE_INTERPRETER_IGNORE_H
