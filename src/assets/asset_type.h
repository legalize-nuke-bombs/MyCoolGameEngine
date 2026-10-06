#ifndef MYCOOLGAMEENGINE_ASSET_TYPE_H
#define MYCOOLGAMEENGINE_ASSET_TYPE_H

struct parser;

struct asset_type {
    const char *key;
    void (*on_add)(struct parser *parser);
    void (*on_clear)(void);
    void (*on_destroy)(void);
};

#endif //MYCOOLGAMEENGINE_ASSET_TYPE_H
