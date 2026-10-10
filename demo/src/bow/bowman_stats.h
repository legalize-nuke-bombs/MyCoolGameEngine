//
// Created by nikita on 09.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BOWMAN_STATS_H
#define MYCOOLGAMEENGINE_BOWMAN_STATS_H

#include "../health/damage.h"

struct bowman_stats {
    struct damage damage;
    double interval;
    double speed;
    double range;
    int arrows;
};

#endif //MYCOOLGAMEENGINE_BOWMAN_STATS_H
