//
// Created by Nikita on 25.09.2026.
//

#include "subsystem_collection.h"

#include <stdlib.h>
#include <_string.h>

#include "subsystem.h"
#include "../devices/keyboard.h"
#include "../logging/logger.h"
#include "../scene/scene.h"
#include "../utils/list.h"
#include "../utils/dictionary.h"
#include "../utils/string_dictionary.h"
#include "../engine/events/engine_events.h"
#include "../engine/utils/engine_restarter.h"
#include "../interpreter/interpreter.h"
#include "../profiler/profiler.h"
#include "../rendering/renderer.h"
#include "../engine/utils/engine_closer.h"
#include "../engine/utils/engine_restarter.h"
#include "../engine/lifecycle/engine_lifecycle.h"


struct subsystem_collection {
    struct list* list;
    struct dictionary* dict;
};


static void subsystem_collection_capture(const struct subsystem_collection* this, struct subsystem* subsystem) {
    if (subsystem == NULL) {
        logger_error("Subsystem collection received nullptr instead of subsystem");
        return;
    }
    const char* subsystem_name = subsystem_get_name(subsystem);
    if (dictionary_try_add(this->dict, (void*)subsystem_name, subsystem)) {
        list_add(this->list, subsystem);
        logger_info("Subsystem collection captured subsystem `%s`", subsystem_name);
    }
    else {
        logger_error("Subsystem collection failed to capture subsystem `%s`", subsystem_name);
        subsystem_destroy(subsystem);
    }
}

static void subsystem_collection_capture_all(const struct subsystem_collection *this) {
    subsystem_collection_capture(this, engine_lifecycle_create(this));
    subsystem_collection_capture(this, engine_events_create(this));
    subsystem_collection_capture(this, keyboard_create(this));
    subsystem_collection_capture(this, scene_create(strdup("Default scene"), this));
    subsystem_collection_capture(this, renderer_create(this));
    subsystem_collection_capture(this, interpreter_create(this));
    subsystem_collection_capture(this, profiler_create(this));
    subsystem_collection_capture(this, engine_closer_create(this));
    subsystem_collection_capture(this, engine_restarter_create(this));
}


struct subsystem_collection* subsystem_collection_create() {
    logger_info("Subsystem collection is creating...");
    struct subsystem_collection* this = malloc(sizeof(struct subsystem_collection));
    this->list = list_create(1024);
    this->dict = string_dictionary_build(10);
    subsystem_collection_capture_all(this);
    return this;
}
void subsystem_collection_destroy(struct subsystem_collection* this) {
    logger_info("Subsystem collection is destroying...");
    for (int i = list_count(this->list) - 1; i >= 0; i--) {
        subsystem_destroy(list_get(this->list, i));
    }
    list_destroy(this->list);
    dictionary_destroy(this->dict);
    free(this);
}

void subsystem_collection_enable_all(const struct subsystem_collection* this, struct engine_arguments args) {
    logger_info("Subsystem collection is enabling all subsystems (%d)....", list_count(this->list));
    for (int i = 0; i < list_count(this->list); i++) {
        subsystem_enable(list_get(this->list, i), args);
    }
}
void subsystem_collection_disable_all(const struct subsystem_collection* this) {
    logger_info("Subsystem collection is disabling all subsystems (%d)...", list_count(this->list));
    for (int i = list_count(this->list) - 1; i >= 0; i--) {
        subsystem_disable(list_get(this->list, i));
    }
}

struct subsystem* subsystem_collection_get(const struct subsystem_collection* this, const char* name) {
    struct subsystem* result = dictionary_get(this->dict, (void*)name);
    if (result == NULL) {
        logger_error("Subsystem collection failed to find subsystem %s", name);
    }
    return result;
}