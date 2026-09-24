//
// Created by nikita on 23.09.2026.
//

#include "parser.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../logging/logger.h"

#define WORD_MAX_LEN 128

struct parser {
    FILE *file;
    char word[WORD_MAX_LEN];
};

struct parser* parser_create(const char* filename) {
    struct parser* parser = malloc(sizeof(struct parser));
    if (parser == NULL) {
        return NULL;
    }

    parser->file = fopen(filename, "r");
    if (parser->file == NULL) {
        logger_warn("Parser failed to open %s", filename);
        free(parser);
        return NULL;
    }
    return parser;
}

void parser_destroy(struct parser* parser) {
    if (parser != NULL) {
        if (parser->file != NULL) {
            fclose(parser->file);
        }
        free(parser);
    }
}

const char* parser_next(struct parser* parser) {
    if (fscanf(parser->file, "%s", parser->word) == 1) {
        return parser->word;
    }
    return NULL;
}
char* parser_next_dup(struct parser* parser) {
    if (fscanf(parser->file, "%s", parser->word) == 1) {
        return strdup(parser->word);
    }
    return NULL;
}

int parser_next_int(const struct parser* parser, int* out_value) {
    if (fscanf(parser->file, "%d", out_value) == 1) {
        return 1;
    }
    return 0;
}
int parser_next_char(const struct parser* parser, char* out_value) {
    if (fscanf(parser->file, "%c", out_value) == 1) {
        return 1;
    }
    return 0;
}
uint8_t parser_next_uint8(const struct parser* parser, uint8_t* out_value) {
    if (fscanf(parser->file, "%" SCNu8, out_value) == 1) {
        return 1;
    }
    return 0;
}
int parser_next_double(const struct parser* parser, double* out_value) {
    if (fscanf(parser->file, "%lf", out_value) == 1) {
        return 1;
    }
    return 0;
}


