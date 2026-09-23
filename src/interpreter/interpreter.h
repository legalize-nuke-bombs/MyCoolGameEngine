//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_H
#define MYCOOLGAMEENGINE_INTERPRETER_H


#define INTERPRETER_OK 0
#define INTERPRETER_FAILED_OPEN_SCRIPT 1

struct interpreter;
struct engine_context;

struct interpreter* interpreter_create(const struct engine_context *engine_context);
void interpreter_destroy(struct interpreter* interpreter);

int interpreter_eval(const struct interpreter *this, const char* script_path);

#endif //MYCOOLGAMEENGINE_INTERPRETER_H
