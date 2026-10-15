//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_PROFILER_H
#define MYCOOLGAMEENGINE_PROFILER_H

#include "../api.h"

struct msystem;

MCGE_API extern const struct msystem profiler_msystem;

MCGE_API const struct time_estimator* profiler_get_frame();
MCGE_API const struct time_estimator* profiler_get_update();
MCGE_API const struct time_estimator* profiler_get_rendering();

#endif //MYCOOLGAMEENGINE_PROFILER_H
