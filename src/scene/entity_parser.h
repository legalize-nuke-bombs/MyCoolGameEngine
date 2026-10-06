#ifndef MYCOOLGAMEENGINE_ENTITY_PARSER_H
#define MYCOOLGAMEENGINE_ENTITY_PARSER_H

struct entity;
struct parser;

struct entity* entity_parse(struct parser *parser, char *name);

#endif //MYCOOLGAMEENGINE_ENTITY_PARSER_H
