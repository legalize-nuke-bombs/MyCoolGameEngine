//
// Created by Nikita on 04.10.2026.
//

#ifndef MYCOOLGAMEENGINE_PLAYER_RUN_AWAY_H
#define MYCOOLGAMEENGINE_PLAYER_RUN_AWAY_H

struct parser;

#define BT_NODE_PLAYER_RUN_AWAY "player_run_away"

struct bt_node* player_run_away_create(double speed);
struct bt_node* player_run_away_parse(struct parser *parser);

#endif //MYCOOLGAMEENGINE_PLAYER_RUN_AWAY_H
