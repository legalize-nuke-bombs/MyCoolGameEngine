//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_INTERPRETER_H
#define MYCOOLGAMEENGINE_INTERPRETER_H

#include "../api.h"


#define INTERPRETER_OK 0
#define INTERPRETER_FAILED_OPEN_SCRIPT 1

struct msystem;
struct action;

MCGE_API extern const struct msystem interpreter_msystem;

MCGE_API int interpreter_eval(const char* script_path);

MCGE_API struct action* interpreter_get_action_on_script_evaluated(void);

#endif //MYCOOLGAMEENGINE_INTERPRETER_H
