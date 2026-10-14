//
// Created by nikita on 14.10.2026.
//

#include "text_module.h"
#include "../../engine/msystem.h"
#include <SDL3_ttf/SDL_ttf.h>


static void text_on_create(void) {
    TTF_Init();
}


static void text_on_destroy(void) {
    TTF_Quit();
}


const struct msystem text_msystem = {
    .name = "text",
    .on_create = text_on_create,
    .on_destroy = text_on_destroy
};