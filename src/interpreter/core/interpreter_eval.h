//
// Created by Nikita on 30.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_EVAL_H
#define MYCOOLGAMEENGINE_INTERPRETER_EVAL_H

struct interpreter_eval;
struct interpreter_command;

struct interpreter_eval* interpreter_eval_create();

struct interpreter_command* interpreter_eval_as_interpreter_command(struct interpreter_eval* this);

#endif //MYCOOLGAMEENGINE_INTERPRETER_EVAL_H
