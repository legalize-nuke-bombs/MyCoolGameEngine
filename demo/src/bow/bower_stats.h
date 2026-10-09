//
// Created by nikita on 09.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BOWER_STATS_H
#define MYCOOLGAMEENGINE_BOWER_STATS_H

#include "../health/damage.h"

struct bower_stats {
    struct damage damage;
    double interval;
    double speed;
    double range;
};

#endif //MYCOOLGAMEENGINE_BOWER_STATS_H
