//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_H
#define MYCOOLGAMEENGINE_INTERPRETER_H


#define INTERPRETER_OK 0
#define INTERPRETER_FAILED_OPEN_SCRIPT 1

struct interpreter;
struct subsystem_collection;
struct action;

struct subsystem* interpreter_create(const struct subsystem_collection *subsystems);

int interpreter_eval(const struct interpreter *this, const char* script_path);

struct action* interpreter_get_action_on_script_evaluated(struct interpreter *this);

#endif //MYCOOLGAMEENGINE_INTERPRETER_H
