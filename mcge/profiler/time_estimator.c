//
// Created by nikita on 24.09.2026.
//

#include "time_estimator.h"

#include <stdlib.h>
#include <SDL3/SDL.h>


struct time_estimator {
    unsigned long long blocks_num;
    unsigned long long blocks_sum;
    unsigned long long blocks_last_average;

    unsigned long long last_block_start;
    unsigned long long last_consolidation;
};

struct time_estimator* time_estimator_create() {
    struct time_estimator* this = malloc(sizeof(struct time_estimator));

    this->blocks_num = 0;
    this->blocks_sum = 0;
    this->blocks_last_average = -1;

    this->last_block_start = -1;
    this->last_consolidation = -1;

    return this;
}
void time_estimator_destroy(struct time_estimator* this) {
    free(this);
}

void time_estimator_start_block(struct time_estimator* this) {
    const unsigned long long now = SDL_GetTicksNS();
    if (this->last_block_start != -1) {
        const unsigned long long delta = now - this->last_block_start;
        this->blocks_num++;
        this->blocks_sum += delta;
    }
    this->last_block_start = SDL_GetTicksNS();
}
void time_estimator_stop_block(struct time_estimator* this) {
    if (this->last_block_start != -1) {
        const unsigned long long delta = SDL_GetTicksNS() - this->last_block_start;
        this->blocks_num++;
        this->blocks_sum += delta;
        this->last_block_start = -1;
    }
}

void time_estimator_update(struct time_estimator* this) {
    const unsigned long long now = SDL_GetTicksNS();
    if (this->last_consolidation == -1 || now - this->last_consolidation >= (int)1e+9) {
        if (this->blocks_num == 0) {
            this->blocks_last_average = 0;
        }
        else {
            this->blocks_last_average = this->blocks_sum / this->blocks_num;
        }
        this->blocks_num = 0;
        this->blocks_sum = 0;
        this->last_consolidation = now;
    }
}

unsigned long long time_estimator_average_block_ns(const struct time_estimator* this) {
    if (this->blocks_last_average == -1) {
        return 0;
    }
    return this->blocks_last_average;
}
double time_estimator_average_block_ms(const struct time_estimator* this) {
    return (double)time_estimator_average_block_ns(this) / 1e+6;
}