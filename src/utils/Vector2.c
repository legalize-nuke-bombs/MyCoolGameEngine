#include "Vector2.h"

struct Vector2 vector2_0() {
    struct Vector2 vector;
    vector.x = 0;
    vector.y = 0;
    return vector;
}
struct Vector2 vector2_1() {
    struct Vector2 vector;
    vector.x = 1;
    vector.y = 1;
    return vector;
}

double get_vector_x(const struct Vector2 *vector) {
    return vector->x;
}
double get_vector_y(const struct Vector2 *vector) {
    return vector->y;
}
void set_vector_x(struct Vector2 *vector, double x) {
    vector->x = x;
}
void set_vector_y(struct Vector2 *vector, double y) {
    vector->y = y;
}