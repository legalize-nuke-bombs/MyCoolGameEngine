#ifndef MYCOOLGAMEENGINE_ENTITY_PARSER_H
#define MYCOOLGAMEENGINE_ENTITY_PARSER_H

struct entity;
struct parser;
struct scene;

struct entity* entity_parse(struct parser *parser, struct scene *scene, char *name);

#endif //MYCOOLGAMEENGINE_ENTITY_PARSER_H
