//
// Created by Nikita on 27.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TEXTURE_H
#define MYCOOLGAMEENGINE_TEXTURE_H

struct texture;

struct texture* texture_create(char* id, char* path);
void texture_destroy(struct texture* this);

struct SDL_Texture* texture_get_native_texture(struct texture* this);

#endif //MYCOOLGAMEENGINE_TEXTURE_H
