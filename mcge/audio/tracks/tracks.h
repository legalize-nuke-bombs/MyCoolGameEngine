//
// Created by nikita on 11.10.2026.
//

#ifndef MYCOOLGAMEENGINE_TRACKS_H
#define MYCOOLGAMEENGINE_TRACKS_H

#include "../../api.h"
#include "track.h"

struct tracks;
struct MIX_Track;

MCGE_API struct tracks* tracks_create();
MCGE_API void tracks_destroy(struct tracks* this);

MCGE_API struct track tracks_request_track(struct tracks* this);
MCGE_API void tracks_return_track(struct tracks* this, struct track track);

#endif //MYCOOLGAMEENGINE_TRACKS_H
