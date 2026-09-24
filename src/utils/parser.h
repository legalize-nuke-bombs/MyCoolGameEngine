//
// Created by nikita on 23.09.2026.
//

#ifndef MYCOOLGAMEENGINE_PARSER_H
#define MYCOOLGAMEENGINE_PARSER_H
#include <stdint.h>

struct parser;

struct parser* parser_create(const char* filename);
void parser_destroy(struct parser* parser);

const char* parser_next(struct parser* parser);
char* parser_next_dup(struct parser* parser);

int parser_next_int(const struct parser* parser, int* out_value);
uint8_t parser_next_uint8(const struct parser* parser, uint8_t* out_value);
int parser_next_double(const struct parser* parser, double* out_value);

#endif //MYCOOLGAMEENGINE_PARSER_H
