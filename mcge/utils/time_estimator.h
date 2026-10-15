//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TIME_ESTIMATOR_H
#define MYCOOLGAMEENGINE_TIME_ESTIMATOR_H

#include "../api.h"

struct time_estimator;

MCGE_API struct time_estimator* time_estimator_create();
MCGE_API void time_estimator_destroy(struct time_estimator* this);

MCGE_API void time_estimator_start_block(struct time_estimator* this);
MCGE_API void time_estimator_stop_block(struct time_estimator* this);

MCGE_API void time_estimator_update(struct time_estimator* this);

MCGE_API unsigned long long time_estimator_average_block_ns(const struct time_estimator* this);
MCGE_API double time_estimator_average_block_ms(const struct time_estimator* this);


#endif //MYCOOLGAMEENGINE_TIME_ESTIMATOR_H
