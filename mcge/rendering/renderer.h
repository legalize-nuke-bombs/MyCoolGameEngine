//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RENDERER_H
#define MYCOOLGAMEENGINE_RENDERER_H

#include <stdbool.h>

#include "../api.h"

struct msystem;

MCGE_API extern const struct msystem renderer_msystem;

MCGE_API struct SDL_Renderer* renderer_get_native_renderer(void);
MCGE_API struct renderer_pipeline* renderer_get_pipeline(void);
MCGE_API struct action* renderer_get_post_pipeline(void);

MCGE_API bool renderer_is_fullscreen(void);
MCGE_API void renderer_set_fullscreen(bool fullscreen);

#endif //MYCOOLGAMEENGINE_RENDERER_H
