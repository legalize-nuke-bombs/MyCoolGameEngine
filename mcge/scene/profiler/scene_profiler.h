//
// Created by nikita on 15.10.2026.
//

#ifndef MYCOOLGAMEENGINE_SCENE_PROFILER_H
#define MYCOOLGAMEENGINE_SCENE_PROFILER_H

struct scene_profiler;

struct scene_profiler* scene_profiler_create();
void scene_profiler_destroy(struct scene_profiler* this);

void scene_profiler_clear(const struct scene_profiler* this);

void scene_profiler_start_component(const struct scene_profiler *this, const char* component_key);
void scene_profiler_end_component(const struct scene_profiler *this, const char* component_key);

void scene_profiler_update(const struct scene_profiler* this);

#endif //MYCOOLGAMEENGINE_SCENE_PROFILER_H
