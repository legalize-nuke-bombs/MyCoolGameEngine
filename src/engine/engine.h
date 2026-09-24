//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_ENGINE_H
#define MYCOOLGAMEENGINE_ENGINE_H

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
struct scene;


struct engine* engine_create();
void engine_destroy(struct engine *this);

void engine_execute(struct engine *this, const char* script_path);

void engine_capture_window(struct engine *this, SDL_Window *window);
void engine_capture_renderer(struct engine *this, SDL_Renderer *renderer);
struct renderer_pipeline *engine_get_renderer_pipeline(const struct engine *this);

void engine_capture_scene(struct engine *this, struct scene *scene);

#endif //MYCOOLGAMEENGINE_ENGINE_H
