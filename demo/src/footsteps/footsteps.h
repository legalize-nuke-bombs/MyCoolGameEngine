//
// Created by nikita on 12.10.2026.
//

#ifndef MYCOOLGAMEENGINE_FOOTSTEPS_H
#define MYCOOLGAMEENGINE_FOOTSTEPS_H

struct footsteps;

const char* footsteps_component_key(void);

extern const struct component_vtable footsteps_vtable;

struct footsteps_on_footstep_callback_context {
    const struct rigid_surface *surface;
};
struct action* footsteps_get_on_footstep(struct footsteps *this);

#endif //MYCOOLGAMEENGINE_FOOTSTEPS_H
