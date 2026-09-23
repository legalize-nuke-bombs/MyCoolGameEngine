//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_H
#define MYCOOLGAMEENGINE_INTERPRETER_H


#define INTERPRETER_OK 0
#define INTERPRETER_FAILED_OPEN_SCRIPT 1

int interpreter_eval(const char* script_path);

#endif //MYCOOLGAMEENGINE_INTERPRETER_H
