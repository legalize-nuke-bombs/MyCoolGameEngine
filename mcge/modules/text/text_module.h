//
// Created by nikita on 14.10.2026.
//

#ifndef MYCOOLGAMEENGINE_TEXT_MODULE_H
#define MYCOOLGAMEENGINE_TEXT_MODULE_H

#include "../../api.h"

MCGE_API extern const struct msystem text_msystem;

MCGE_API struct TTF_TextEngine* text_module_get_native_text_engine();

#endif //MYCOOLGAMEENGINE_TEXT_MODULE_H
