#include "vector2.h"

struct vector2 vector2_zero() {
    struct vector2 vector;
    vector.x = 0;
    vector.y = 0;
    return vector;
}
struct vector2 vector2_one() {
    struct vector2 vector;
    vector.x = 1;
    vector.y = 1;
    return vector;
}