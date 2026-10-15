#include "sound.h"

#include <stdlib.h>

#include "clips.h"
#include "../../random/random.h"
#include "mcge/memory/allocator.h"


struct sound {
    char *id;
    struct list clip_names;
};


struct sound* sound_create(char *id, const struct list clip_names) {
    struct sound* this = allocator_new0(1, sizeof(struct sound));
    this->id = id;
    this->clip_names = clip_names;
    return this;
}
void sound_destroy(struct sound* this) {
    for (int i = 0; i < list_count(&this->clip_names); i++) {
        allocator_delete(list_get(&this->clip_names, i));
    }
    list_destroy(&this->clip_names);
    allocator_delete(this->id);
    allocator_delete(this);
}

static struct sound sound_fallback = {
    .id = "<fallback>"
};
struct sound* sound_get_fallback(void) {
    return &sound_fallback;
}

const char* sound_get_id(const struct sound* this) {
    return this->id;
}

struct clip* sound_get_random_clip(const struct sound* this) {
    if (this == NULL || list_count(&this->clip_names) == 0) {
        return NULL;
    }
    const char *clip_name = list_get(&this->clip_names, random_next_int(0, list_count(&this->clip_names)));
    return clips_get(clip_name);
}
