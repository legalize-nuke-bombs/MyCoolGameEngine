//
// Created by Nikita on 27.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TEXTURE_MANAGER_H
#define MYCOOLGAMEENGINE_TEXTURE_MANAGER_H

struct texture_manager;
struct texture;

struct texture_manager* texture_manager_create();
void texture_manager_destroy(struct texture_manager* this);

void texture_manager_capture(struct texture_manager* this, struct texture* texture);

struct texture* texture_manager_try_get_texture(const struct texture_manager* this, const char* texture_id);

void texture_manager_clear(struct texture_manager* this);

#endif //MYCOOLGAMEENGINE_TEXTURE_MANAGER_H
