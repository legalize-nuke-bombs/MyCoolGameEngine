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

void engine_execute(struct engine *this);

#endif //MYCOOLGAMEENGINE_ENGINE_H
