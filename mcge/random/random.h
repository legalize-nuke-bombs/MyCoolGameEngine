//
// Created by nikita on 28.09.2026.
//

#ifndef MYCOOLGAMEENGINE_RANDOM_H
#define MYCOOLGAMEENGINE_RANDOM_H

struct msystem;

extern const struct msystem random_msystem;

int random_next_int(int l, int r);
double random_next_double(double l, double r);

#endif //MYCOOLGAMEENGINE_RANDOM_H
