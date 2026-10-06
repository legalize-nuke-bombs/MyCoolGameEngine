#ifndef MYCOOLGAMEENGINE_ASSETS_H
#define MYCOOLGAMEENGINE_ASSETS_H

struct msystem;
struct asset_type;
struct parser;

extern const struct msystem assets_msystem;

void assets_register(const struct asset_type *type);

void assets_add(const char *key, struct parser *parser);

#endif //MYCOOLGAMEENGINE_ASSETS_H
