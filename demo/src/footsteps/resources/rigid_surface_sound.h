//
// Created by nikita on 12.10.2026.
//

#ifndef MYCOOLGAMEENGINE_RIGID_SURFACE_SOUND_H
#define MYCOOLGAMEENGINE_RIGID_SURFACE_SOUND_H

struct rigid_surface_sound* rigid_surface_sound_create(char *id, char *sound_name);
void rigid_surface_sound_destroy(struct rigid_surface_sound *this);

struct sound* rigid_surface_sound_get_sound(struct rigid_surface_sound *this);

#endif //MYCOOLGAMEENGINE_RIGID_SURFACE_SOUND_H
