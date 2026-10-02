//
// Created by nikita on 01.10.2026.
//

#include "rigid_body.h"

#include <stdlib.h>

#include "rigid_surface.h"
#include "../component_internal.h"
#include "../../entity.h"
#include "../../scene.h"
#include "../../../logging/logger.h"
#include "../../../utils/dictionary.h"
#include "../../../utils/parser.h"
#include "../../../utils/vector2_math.h"
#include "../../chunks/chunks.h"


#define FRICTION_DEFAULT 0.5f
#define GRAVITY 320.f


struct rigid_body {
    struct component base;
    double m;
    double base_friction_coefficient;
    double rolling_friction_coefficient;

    struct vector2 v;
    struct vector2 f_sum;

    const struct chunks* chunks;
};

static struct component* rigid_body_clone(struct component base, const struct component *component);
static void rigid_body_awake(struct component *base);
static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context);

static const struct component_vtable rigid_body_vtable = {
    .component_key = rigid_body_component_key,
    .on_clone = rigid_body_clone,
    .on_awake = rigid_body_awake,
    .on_simulation_chunk_update = rigid_body_simulation_chunk_update
};

const char* rigid_body_component_key(void) {
    return "rigid_body";
}

struct component* rigid_body_create(struct parser *parser, struct entity *parent) {
    struct rigid_body *this = calloc(1, sizeof(struct rigid_body));
    struct component *base = (struct component *) this;
    component_base_create(base, &rigid_body_vtable, parent);

    parser_next_double(parser, &this->m);
    if (this->m < 0) this->m = 1e+9;
    parser_next_double(parser, &this->base_friction_coefficient);
    if (this->base_friction_coefficient < 0) this->base_friction_coefficient = 0.1f;
    parser_next_double(parser, &this->rolling_friction_coefficient);
    if (this->rolling_friction_coefficient < 0) this->rolling_friction_coefficient = 0.1f;

    return base;
}

static struct component* rigid_body_clone(struct component base, const struct component *component) {
    const struct rigid_body *rigid_body = (struct rigid_body *) component;

    struct rigid_body* this = calloc(1, sizeof(struct rigid_body));
    this->base = base;
    this->m = rigid_body->m;
    this->base_friction_coefficient = rigid_body->base_friction_coefficient;
    this->rolling_friction_coefficient = rigid_body->rolling_friction_coefficient;
    return (struct component*)this;
}

static void rigid_body_awake(struct component *base) {
    struct rigid_body *this = (struct rigid_body *) base;
    this->chunks = scene_get_chunks(entity_get_scene(component_get_parent(base)));
}

static void rigid_body_apply_force(struct rigid_body *this, const double dt) {
    const struct vector2 a = vector_multiply_scalar(this->f_sum, 1 / this->m);
    this->v = vector_sum(this->v, vector_multiply_scalar(a, dt));
    this->f_sum = vector2_zero;
}

static void rigid_body_apply_rolling_friction(struct rigid_body *this, const double dt) {
    const double v_scalar = vector_mod(this->v);
    if (v_scalar <= 1e-9) {
        this->v = vector2_zero;
        return;
    }
    const struct vector2 v_direction = vector_multiply_scalar(this->v, 1 / v_scalar);
    const double f_input_scalar = this->m * v_scalar / dt;

    const struct vector2 f_friction_direction = vector_multiply_scalar(v_direction, -1);
    const double f_max_friction_scalar = this->m * GRAVITY * this->base_friction_coefficient * this->rolling_friction_coefficient * rigid_surface_get_friction(this->chunks, component_get_rect((struct component*)this));
    const double f_friction_scalar = f_input_scalar >= f_max_friction_scalar ? f_max_friction_scalar : f_input_scalar;
    const struct vector2 f_friction = vector_multiply_scalar(f_friction_direction, f_friction_scalar);

    const struct vector2 a = vector_multiply_scalar(f_friction, 1 / this->m);
    this->v = vector_sum(this->v, vector_multiply_scalar(a, dt));
}

static void rigid_body_move(struct rigid_body *this, const double dt) {
    const struct vector2 d_pos = vector_multiply_scalar(this->v, dt);

    struct entity* parent = component_get_parent((struct component*)this);
    struct rect local_rect = entity_get_local_rect(parent);
    local_rect.position = vector_sum(local_rect.position, d_pos);
    entity_set_local_rect(parent, local_rect);
}

static void rigid_body_simulation_chunk_update(struct component* base, const struct update_context *context) {
    struct rigid_body *this = (struct rigid_body *) base;
    const double dt = context->dt;

    rigid_body_apply_force(this, dt);
    rigid_body_apply_rolling_friction(this, dt);
    rigid_body_move(this, dt);
}

void rigid_body_push(struct rigid_body *this, const struct vector2 impulse) {
    const struct vector2 delta_v = vector_multiply_scalar(impulse, 1.0 / this->m);
    this->v = vector_sum(this->v, delta_v);
}

void rigid_body_push_off(struct rigid_body *this, const struct vector2 f) {
    const double f_scalar = vector_mod(f);
    if (f_scalar <= 1e-9) {
        return;
    }

    // сука это говно какое-то это надо сносить все и заново писать todo

    const struct vector2 f_direction = vector_multiply_scalar(f, 1.0 / f_scalar);

    const double f_friction_scalar_max = this->m * GRAVITY * this->base_friction_coefficient * rigid_surface_get_friction(this->chunks, component_get_rect((struct component*)this));

    const double f_result_scalar = f_scalar >= f_friction_scalar_max ? f_friction_scalar_max : f_scalar;
    const struct vector2 f_result = vector_multiply_scalar(f_direction, f_result_scalar);
    this->f_sum = vector_sum(this->f_sum, f_result);
}

void rigid_body_explosion(struct rect rect, const double f, const struct chunks *chunks) {
    logger_debug("Explosion x %f y %f w %f h %f f %f", rect.position.x, rect.position.y, rect.size.x, rect.size.y, f);

    const double f_min = 10000.f; // TODO fix after physical based coordiantes
    const double n = f / f_min;
    rect.size = vector_multiply_scalar(rect.size, n);

    int x_start, x_end, y_start, y_end;
    chunks_get_rect_indexes(chunks, rect, &x_start, &x_end, &y_start, &y_end);
    for (int x = x_start; x <= x_end; x++) {
        for (int y = y_start; y <= y_end; y++) {
            const struct dictionary *dict = chunks_chunk_get_components_by_type(chunks, x, y, "rigid_body");
            if (dict == NULL) {
                continue;
            }
            struct dictionary_iterator iterator = dictionary_begin(dict);
            struct dictionary_node node;
            while (dictionary_next(dict, &iterator, &node)) {
                struct rigid_body *rb = node.value;
                const struct vector2 rb_position = component_get_rect((struct component*)rb).position;
                const struct vector2 d_pos = vector_sub(rect.position, rb_position);
                double r_sqr = vector_sql_mod(d_pos);
                if (r_sqr < 0.01f) r_sqr = 1e+9;
                struct vector2 f_output = vector2_one;
                f_output = vector_multiply_scalar(f_output, f);
                f_output = vector_multiply_vector(f_output, d_pos);
                f_output = vector_multiply_scalar(f_output, -(1 / r_sqr));
                logger_debug("Explosion pushes entity %s with force %f %f", component_get_global_parent_name((struct component*)rb), f_output.x, f_output.y);
                rigid_body_push(rb, f_output);
            }
        }
    }
}