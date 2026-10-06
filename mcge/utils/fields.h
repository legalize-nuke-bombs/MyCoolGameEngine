#ifndef MYCOOLGAMEENGINE_FIELDS_H
#define MYCOOLGAMEENGINE_FIELDS_H

#include <stdbool.h>

#include "color.h"
#include "vector2.h"

struct fields;
struct fields_list;

struct fields_list* fields_parse_file(const char *filename);
void fields_list_destroy(struct fields_list *this);

struct fields* fields_clone(const struct fields *this);
void fields_destroy(struct fields *this);

int fields_list_count(const struct fields_list *this);
struct fields* fields_list_get(const struct fields_list *this, int index);

const char* fields_key(const struct fields *this);
int fields_line(const struct fields *this);

bool fields_has(const struct fields *this, const char *name);

const char* fields_get_string(struct fields *this, const char *name, const char *default_value);
char* fields_dup_string(struct fields *this, const char *name, const char *default_value);
bool fields_get_bool(struct fields *this, const char *name, bool default_value);
double fields_get_double(struct fields *this, const char *name, double default_value);
int fields_get_int(struct fields *this, const char *name, int default_value);
struct vector2 fields_get_vector2(struct fields *this, const char *name, struct vector2 default_value);
struct color fields_get_color(struct fields *this, const char *name, struct color default_value);
const struct fields_list* fields_get_list(struct fields *this, const char *name);

void fields_warn_unknown(struct fields *this);

#endif //MYCOOLGAMEENGINE_FIELDS_H
