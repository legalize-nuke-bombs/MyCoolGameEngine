//
// Created by nikita on 24.09.2026.
//

#ifndef MYCOOLGAMEENGINE_DEVICES_H
#define MYCOOLGAMEENGINE_DEVICES_H

struct engine;
struct devices;
struct keyboard;

struct devices* devices_create(struct engine* engine);
void devices_destroy(struct devices* this);

void devices_awake(struct devices *this);
void devices_disable(struct devices *this);

struct keyboard* devices_get_keyboard(const struct devices *this);

#endif //MYCOOLGAMEENGINE_DEVICES_H
