//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_LIFECYCLE_H
#define MYCOOLGAMEENGINE_ENGINE_LIFECYCLE_H
#include <stdbool.h>

struct msystem;

extern const struct msystem engine_lifecycle_msystem;

bool engine_lifecycle_is_running(void);
bool engine_lifecycle_restart_required(void);

void engine_lifecycle_mark_stop(void);
void engine_lifecycle_mark_restart(void);

#endif //MYCOOLGAMEENGINE_ENGINE_LIFECYCLE_H
