#ifndef MYCOOLGAMEENGINE_MSYSTEM_H
#define MYCOOLGAMEENGINE_MSYSTEM_H

#include "../engine/engine_arguments.h"

struct msystem {
    const char *name;
    void (*on_create)(void);
    void (*on_enable)(struct engine_arguments args);
    void (*on_disable)(void);
    void (*on_destroy)(void);
};

#endif //MYCOOLGAMEENGINE_MSYSTEM_H
