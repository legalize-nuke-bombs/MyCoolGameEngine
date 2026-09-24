//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_TIME_ESTIMATOR_H
#define MYCOOLGAMEENGINE_TIME_ESTIMATOR_H

struct time_estimator;

struct time_estimator* time_estimator_create();
void time_estimator_destroy(struct time_estimator* this);

void time_estimator_start_block(struct time_estimator* this);
void time_estimator_stop_block(struct time_estimator* this);

unsigned long long time_estimator_average_block_ns(const struct time_estimator* this);


#endif //MYCOOLGAMEENGINE_TIME_ESTIMATOR_H
