//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_PROFILER_H
#define MYCOOLGAMEENGINE_PROFILER_H

struct subsystem_collection;
struct profiler;
struct time_estimator;

struct subsystem* profiler_create(const struct subsystem_collection* subsystems);

#endif //MYCOOLGAMEENGINE_PROFILER_H
