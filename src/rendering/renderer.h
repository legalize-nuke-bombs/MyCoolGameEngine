//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_H
#define MYCOOLGAMEENGINE_RENDERER_H

struct renderer;
struct engine;

struct renderer* renderer_create(struct engine* engine);
void renderer_destroy(struct renderer *this);

void renderer_awake(struct renderer *this);
void renderer_disable(struct renderer *this);

#endif //MYCOOLGAMEENGINE_RENDERER_H
