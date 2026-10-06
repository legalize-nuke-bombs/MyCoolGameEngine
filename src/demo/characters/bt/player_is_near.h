//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_PLAYER_IS_NEAR_H
#define MYCOOLGAMEENGINE_PLAYER_IS_NEAR_H

struct parser;

#define BT_NODE_PLAYER_IS_NEAR "player_is_near"

struct bt_node* player_is_near_create(double radius);
struct bt_node* player_is_near_parse(struct parser *parser);

#endif //MYCOOLGAMEENGINE_PLAYER_IS_NEAR_H
