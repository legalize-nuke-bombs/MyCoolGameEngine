//
// Created by nikita on 15.10.2026.
//

#include "scene_profiler.h"

#include <stdlib.h>

#include "mcge/allocator/allocator.h"
#include "mcge/utils/dictionary.h"
#include "mcge/utils/string_dictionary.h"
#include "mcge/utils/time_estimator.h"


struct scene_profiler {
    struct dictionary* time_estimators;
};


struct scene_profiler* scene_profiler_create() {
    struct scene_profiler* this = allocator_new0(1, sizeof(struct scene_profiler));
    this->time_estimators = string_dictionary_build(4);
    return this;
}
void scene_profiler_destroy(struct scene_profiler* this) {
    scene_profiler_clear(this);
    dictionary_destroy(this->time_estimators);
    allocator_delete(this);
}

void scene_profiler_clear(const struct scene_profiler* this) {
    struct dictionary_iterator iterator = dictionary_begin(this->time_estimators);
    struct dictionary_node node;
    while (dictionary_next(this->time_estimators, &iterator, &node)) {
        struct time_estimator *time_estimator = node.value;
        time_estimator_destroy(time_estimator);
    }
    dictionary_clear(this->time_estimators);
}

void scene_profiler_start_component(const struct scene_profiler *this, const char* component_key) {
    struct time_estimator* time_estimator = dictionary_get(this->time_estimators, (void*)component_key);
    if (time_estimator == NULL) {
        time_estimator = time_estimator_create();
        dictionary_try_add(this->time_estimators, (void*)component_key, time_estimator);
    }
    time_estimator_start_block(time_estimator);
}
void scene_profiler_end_component(const struct scene_profiler *this, const char* component_key) {
    struct time_estimator* time_estimator = dictionary_get(this->time_estimators, (void*)component_key);
    if (time_estimator == NULL) {
        return;
    }
    time_estimator_stop_block(time_estimator);
}


void scene_profiler_update(const struct scene_profiler* this) {
    struct dictionary_iterator iterator = dictionary_begin(this->time_estimators);
    struct dictionary_node node;
    while (dictionary_next(this->time_estimators, &iterator, &node)) {
        struct time_estimator *time_estimator = node.value;
        time_estimator_update(time_estimator);
    }
}


