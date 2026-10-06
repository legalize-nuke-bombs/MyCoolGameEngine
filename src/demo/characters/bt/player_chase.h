//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_PLAYER_CHASE_H
#define MYCOOLGAMEENGINE_PLAYER_CHASE_H

struct parser;

#define BT_NODE_PLAYER_CHASE "player_chase"

struct bt_node* player_chase_create(double speed);
struct bt_node* player_chase_parse(struct parser *parser);

#endif //MYCOOLGAMEENGINE_PLAYER_CHASE_H
