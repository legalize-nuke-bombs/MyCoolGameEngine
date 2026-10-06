//
// Created by nikita on 03.10.2026.
//

#include "skill_factory.h"

#include <stddef.h>
#include <string.h>

#include "../../../logging/logger.h"
#include "custom/fear_balls/fear_balls.h"
#include "custom/printer.h"

static const struct {
    const char *key;
    struct skill* (*parse)(struct parser *parser, struct entity *self);
} skills[] = {
    { SKILL_PRINTER_KEY, printer_parse },
    { SKILL_FEAR_BALLS_KEY, fear_balls_parse },
};

struct skill* skill_factory_produce(const char* key, struct parser *parser, struct entity *self) {
    for (size_t i = 0; i < sizeof(skills) / sizeof(skills[0]); i++) {
        if (key != NULL && strcmp(skills[i].key, key) == 0) {
            return skills[i].parse(parser, self);
        }
    }
    logger_warn("Skill factory does not know %s", key ? key : "<null>");
    return NULL;
}
