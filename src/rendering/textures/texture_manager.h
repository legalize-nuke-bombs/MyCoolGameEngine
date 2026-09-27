//
// Created by Nikita on 27.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TEXTURE_MANAGER_H
#define MYCOOLGAMEENGINE_TEXTURE_MANAGER_H

struct texture_manager;

struct texture_manager* texture_manager_create();
void texture_manager_destroy(struct texture_manager* this);

#endif //MYCOOLGAMEENGINE_TEXTURE_MANAGER_H
