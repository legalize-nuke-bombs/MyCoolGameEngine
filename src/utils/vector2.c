#include "vector2.h"

struct vector2 vector2_0() {
    struct vector2 vector;
    vector.x = 0;
    vector.y = 0;
    return vector;
}
struct vector2 vector2_1() {
    struct vector2 vector;
    vector.x = 1;
    vector.y = 1;
    return vector;
}

double get_vector_x(const struct vector2 *vector) {
    return vector->x;
}
double get_vector_y(const struct vector2 *vector) {
    return vector->y;
}
void set_vector_x(struct vector2 *vector, double x) {
    vector->x = x;
}
void set_vector_y(struct vector2 *vector, double y) {
    vector->y = y;
}