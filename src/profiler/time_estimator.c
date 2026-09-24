//
// Created by nikita on 24.09.2026.
//

#include "time_estimator.h"

#include <stdlib.h>
#include <SDL3/SDL.h>


struct time_estimator {
    unsigned long long blocks;
    unsigned long long blocks_ns;
    unsigned long long last_block_start;
};

struct time_estimator* time_estimator_create() {
    struct time_estimator* this = malloc(sizeof(struct time_estimator));
    this->blocks = 0;
    this->blocks_ns = 0;
    this->last_block_start = -1;
    return this;
}
void time_estimator_destroy(struct time_estimator* this) {
    free(this);
}

void time_estimator_start_block(struct time_estimator* this) {
    const unsigned long long now = SDL_GetTicksNS();
    if (this->last_block_start != -1) {
        const unsigned long long delta = now - this->last_block_start;
        this->blocks++;
        this->blocks_ns += delta;
    }
    this->last_block_start = SDL_GetTicksNS();
}
void time_estimator_stop_block(struct time_estimator* this) {
    if (this->last_block_start != -1) {
        const unsigned long long delta = SDL_GetTicksNS() - this->last_block_start;
        this->blocks++;
        this->blocks_ns += delta;
        this->last_block_start = -1;
    }
}

unsigned long long time_estimator_average_block_ns(const struct time_estimator* this) {
    if (this->blocks == 0) {
        return 0;
    }
    return this->blocks_ns / this->blocks;
}
double time_estimator_average_block_ms(const struct time_estimator* this) {
    return (double)time_estimator_average_block_ns(this) / 1e+6;
}
double time_estimator_average_block_fps(const struct time_estimator* this) {
    return 1e+3 / time_estimator_average_block_ms(this);
}