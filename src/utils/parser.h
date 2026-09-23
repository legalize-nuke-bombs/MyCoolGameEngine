//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_PARSER_H
#define MYCOOLGAMEENGINE_PARSER_H

struct parser;

struct parser* parser_create(const char* filename);
void parser_destroy(struct parser* parser);

char* parser_next(struct parser* parser);

#endif //MYCOOLGAMEENGINE_PARSER_H
