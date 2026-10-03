//
// Created by nikita on 03.10.2026.
//

#ifndef MYCOOLGAMEENGINE_PRINTER_H
#define MYCOOLGAMEENGINE_PRINTER_H

struct parser;
struct entity;

#define SKILL_PRINTER_KEY "printer"

struct skill* printer_parse(struct parser *parser, struct entity *self);

#endif //MYCOOLGAMEENGINE_PRINTER_H
