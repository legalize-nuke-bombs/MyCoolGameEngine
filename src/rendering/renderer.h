//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_H
#define MYCOOLGAMEENGINE_RENDERER_H

struct msystem;

extern const struct msystem renderer_msystem;

struct SDL_Renderer* renderer_get_native_renderer(void);
struct renderer_pipeline* renderer_get_pipeline(void);

#endif //MYCOOLGAMEENGINE_RENDERER_H
