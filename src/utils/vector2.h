#ifndef MYCOOLGAMEENGINE_VECTOR2_H
#define MYCOOLGAMEENGINE_VECTOR2_H

struct vector2 {
    double x;
    double y;
};

struct vector2 vector2_0();
struct vector2 vector2_1();

double get_vector_x(const struct vector2 *vector);
double get_vector_y(const struct vector2 *vector);
void set_vector_x(struct vector2 *vector, double x);
void set_vector_y(struct vector2 *vector, double y);

#endif
