#ifndef MYCOOLGAMEENGINE_FIELDS_H
#define MYCOOLGAMEENGINE_FIELDS_H

#include <stdbool.h>

#include "color.h"
#include "vector2.h"
#include "../api.h"

struct fields;
struct fields_list;

MCGE_API struct fields_list* fields_parse_file(const char *filename);
MCGE_API void fields_list_destroy(struct fields_list *this);

MCGE_API struct fields* fields_clone(const struct fields *this);
MCGE_API void fields_destroy(struct fields *this);

MCGE_API int fields_list_count(const struct fields_list *this);
MCGE_API struct fields* fields_list_get(const struct fields_list *this, int index);

MCGE_API const char* fields_key(const struct fields *this);
MCGE_API int fields_line(const struct fields *this);

MCGE_API bool fields_has(const struct fields *this, const char *name);

MCGE_API const char* fields_get_string(struct fields *this, const char *name, const char *default_value);
MCGE_API char* fields_dup_string(struct fields *this, const char *name, const char *default_value);
MCGE_API bool fields_get_bool(struct fields *this, const char *name, bool default_value);
MCGE_API double fields_get_double(struct fields *this, const char *name, double default_value);
MCGE_API int fields_get_int(struct fields *this, const char *name, int default_value);
MCGE_API struct vector2 fields_get_vector2(struct fields *this, const char *name, struct vector2 default_value);
MCGE_API struct color fields_get_color(struct fields *this, const char *name, struct color default_value);
MCGE_API const struct fields_list* fields_get_list(struct fields *this, const char *name);

MCGE_API void fields_warn_unknown(struct fields *this);

#endif //MYCOOLGAMEENGINE_FIELDS_H
