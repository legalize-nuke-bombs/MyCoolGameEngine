//
// Created by Nikita on 25.09.2026.
//

#ifndef MYCOOLGAMEENGINE_SUBSYSTEM_H
#define MYCOOLGAMEENGINE_SUBSYSTEM_H

struct subsystem;

void subsystem_destroy(struct subsystem *this);

void subsystem_enable(struct subsystem *this);
void subsystem_disable(struct subsystem *this);

const char* subsystem_get_name(struct subsystem *this);

#endif //MYCOOLGAMEENGINE_SUBSYSTEM_H
