#ifndef MYCOOLGAMEENGINE_SOUND_H
#define MYCOOLGAMEENGINE_SOUND_H

#include "../api.h"
#include "../utils/list.h"

struct sound;
struct clip;

MCGE_API struct sound* sound_create(char *id, struct list clip_names);
MCGE_API void sound_destroy(struct sound* this);

MCGE_API const char* sound_get_id(const struct sound* this);

MCGE_API struct clip* sound_get_random_clip(const struct sound* this);

#endif //MYCOOLGAMEENGINE_SOUND_H
