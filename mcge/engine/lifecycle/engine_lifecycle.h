//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_LIFECYCLE_H
#define MYCOOLGAMEENGINE_ENGINE_LIFECYCLE_H
#include <stdbool.h>
#include "../../api.h"

struct msystem;

MCGE_API extern const struct msystem engine_lifecycle_msystem;

MCGE_API bool engine_lifecycle_is_running(void);
MCGE_API bool engine_lifecycle_restart_required(void);

MCGE_API void engine_lifecycle_mark_stop(void);
MCGE_API void engine_lifecycle_mark_restart(void);

#endif //MYCOOLGAMEENGINE_ENGINE_LIFECYCLE_H
