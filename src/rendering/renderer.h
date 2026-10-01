//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_H
#define MYCOOLGAMEENGINE_RENDERER_H

struct renderer;
struct subsystem_collection;

struct subsystem* renderer_create(const struct subsystem_collection* subsystems);

struct SDL_Renderer* renderer_get_native_renderer(const struct renderer* renderer);
struct renderer_pipeline* renderer_get_pipeline(const struct renderer* this);

#endif //MYCOOLGAMEENGINE_RENDERER_H
