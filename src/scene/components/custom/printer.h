#ifndef MYCOOLGAMEENGINE_PRINTER_H
#define MYCOOLGAMEENGINE_PRINTER_H

struct entity;
struct parser;

const char* printer_component_key(void);

struct component* printer_create(struct parser *parser, struct entity *parent);

#endif
