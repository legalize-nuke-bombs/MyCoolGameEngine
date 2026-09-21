#ifndef MYCOOLGAMEENGINE_VECTOR2_H
#define MYCOOLGAMEENGINE_VECTOR2_H

struct Vector2 {
    double x;
    double y;
};

struct Vector2 vector2_0();
struct Vector2 vector2_1();

double get_vector_x(const struct Vector2 *vector);
double get_vector_y(const struct Vector2 *vector);

#endif
