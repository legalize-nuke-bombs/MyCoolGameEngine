//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_H
#define MYCOOLGAMEENGINE_INTERPRETER_H


#define INTERPRETER_OK 0
#define INTERPRETER_FAILED_OPEN_SCRIPT 1

struct interpreter;
struct engine;

struct interpreter* interpreter_create(struct engine *engine);
void interpreter_destroy(struct interpreter* this);

int interpreter_eval(const struct interpreter *this, const char* script_path);

#endif //MYCOOLGAMEENGINE_INTERPRETER_H
