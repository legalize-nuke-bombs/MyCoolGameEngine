//
// Created by nikita on 14.10.2026.
//

#include "text_module.h"
#include "../../engine/msystem.h"


static void text_on_create(void) {

}


static void text_on_destroy(void) {

}


const struct msystem text_msystem = {
    .name = "text",
    .on_create = text_on_create,
    .on_destroy = text_on_destroy
};