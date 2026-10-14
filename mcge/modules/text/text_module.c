//
// Created by nikita on 14.10.2026.
//

#include "text_module.h"
#include "../../engine/msystem.h"
#include <SDL3_ttf/SDL_ttf.h>

#include "components/text_renderer.h"
#include "fonts/fonts.h"
#include "mcge/assets/assets.h"
#include "mcge/logging/logger.h"
#include "mcge/rendering/renderer.h"
#include "mcge/scene/components/component_factory.h"


static struct {
    TTF_TextEngine *text_engine;
} text_module;


static void text_on_create(void) {
    logger_info("Text module is installing...");
    if (!TTF_Init()) {
        logger_error("Failed to init SDL TTF");
        return;
    }
    text_module.text_engine = TTF_CreateRendererTextEngine(renderer_get_native_renderer());
    if (text_module.text_engine == NULL) {
        logger_error("Failed to create TTF text engine");
        return;
    }

    assets_register(&fonts_asset_type);

    component_factory_register(&text_renderer_vtable);
}


static void text_on_destroy(void) {
    TTF_DestroyRendererTextEngine(text_module.text_engine);
    TTF_Quit();
}


const struct msystem text_msystem = {
    .name = "text",
    .on_create = text_on_create,
    .on_destroy = text_on_destroy
};

TTF_TextEngine* text_module_get_native_text_engine() {
    return text_module.text_engine;
}