#ifndef MYCOOLGAMEENGINE_CATALOGS_H
#define MYCOOLGAMEENGINE_CATALOGS_H

struct catalog;
struct msystem;

extern const struct msystem catalogs_msystem;

struct catalog* catalogs_get(const char *key);
void* catalogs_try_get_item(const char *key, const char *name);
void* catalogs_get_item(const char *key, const char *name);

#endif //MYCOOLGAMEENGINE_CATALOGS_H
