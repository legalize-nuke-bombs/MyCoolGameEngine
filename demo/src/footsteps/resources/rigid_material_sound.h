//
// Created by nikita on 12.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_MATERIAL_SOUND_H
#define MYCOOLGAMEENGINE_RIGID_MATERIAL_SOUND_H

struct rigid_material_sound* rigid_material_sound_create(char *id, char *sound_name);
void rigid_material_sound_destroy(struct rigid_material_sound *this);

struct sound* rigid_material_sound_get_sound(const struct rigid_material_sound *this);

#endif //MYCOOLGAMEENGINE_RIGID_MATERIAL_SOUND_H
