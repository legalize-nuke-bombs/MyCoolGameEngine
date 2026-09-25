//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_PROFILER_H
#define MYCOOLGAMEENGINE_PROFILER_H

struct engine;
struct profiler;
struct time_estimator;

struct profiler* profiler_create(struct engine *engine);
void profiler_destroy(struct profiler* this);

void profiler_awake(struct profiler *this);
void profiler_disable(struct profiler *this);

#endif //MYCOOLGAMEENGINE_PROFILER_H
