#ifndef MYCOOLGAMEENGINE_CATALOGS_H
#define MYCOOLGAMEENGINE_CATALOGS_H

struct catalogs;
struct catalog;
struct subsystem;
struct subsystem_collection;

struct subsystem* catalogs_create(const struct subsystem_collection *subsystems);

struct catalog* catalogs_get(const struct catalogs *this, const char *key);
void* catalogs_get_item(const struct catalogs *this, const char *key, const char *name);

#endif //MYCOOLGAMEENGINE_CATALOGS_H
