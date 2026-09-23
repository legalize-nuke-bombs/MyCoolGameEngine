//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_H
#define MYCOOLGAMEENGINE_ENGINE_H

struct renderer_pipeline;

struct engine* engine_create();
void engine_destroy(struct engine *this);

void engine_run(struct engine *this, const char* script_path);

struct renderer_pipeline *engine_get_renderer_pipeline(const struct engine *this);

#endif //MYCOOLGAMEENGINE_ENGINE_H
