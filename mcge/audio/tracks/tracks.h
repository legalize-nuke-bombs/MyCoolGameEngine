//
// Created by nikita on 11.10.2026.
//

#ifndef MYCOOLGAMEENGINE_TRACKS_H
#define MYCOOLGAMEENGINE_TRACKS_H

#include <stdbool.h>

#include "../../api.h"
#include "track.h"

struct tracks;
struct MIX_Audio;
struct MIX_Point3D;

MCGE_API struct tracks* tracks_create();
MCGE_API void tracks_destroy(struct tracks* this);

MCGE_API struct track tracks_play(struct tracks* this, struct MIX_Audio *audio, const struct MIX_Point3D *position, bool loop);
MCGE_API bool tracks_is_track_playing(const struct tracks* this, const struct track *track);
MCGE_API void tracks_set_track_position(struct tracks* this, const struct track *track, const struct MIX_Point3D *position);
MCGE_API void tracks_stop_track(struct tracks* this, const struct track *track);

#endif //MYCOOLGAMEENGINE_TRACKS_H
