//
// Created by Nikita on 27.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TEXTURE_H
#define MYCOOLGAMEENGINE_TEXTURE_H

#include "texture_loading_mode.h"

struct texture;

struct texture* texture_create(char* id, char* path, enum texture_loading_mode loading_mode);
void texture_destroy(struct texture* this);

const char* texture_get_id(const struct texture* this);
struct SDL_Texture* texture_get_native_texture(struct texture* this);

#endif //MYCOOLGAMEENGINE_TEXTURE_H
