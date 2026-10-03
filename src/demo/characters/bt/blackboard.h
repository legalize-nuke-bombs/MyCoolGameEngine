//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_BLACKBOARD_H
#define MYCOOLGAMEENGINE_BLACKBOARD_H

struct entity;

struct blackboard {
    struct entity *self;
    struct player *target_player;
};

#endif //MYCOOLGAMEENGINE_BLACKBOARD_H
