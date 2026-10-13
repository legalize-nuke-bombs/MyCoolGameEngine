//
// Created by nikita on 13.10.2026.
//

#ifndef MYCOOLGAMEENGINE_WALLET_H
#define MYCOOLGAMEENGINE_WALLET_H
#include <stdbool.h>

struct wallet;

const char* wallet_component_key(void);

extern const struct component_vtable wallet_vtable;

int wallet_amount(const struct wallet *this);
bool wallet_try_take(struct wallet *this, int amount);
void wallet_put(struct wallet *this, int amount);

struct action* wallet_get_on_changed(struct wallet *this);

#endif //MYCOOLGAMEENGINE_WALLET_H
