//
// Created by nikita on 23.09.2026.
//

#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include "../logging/logger.h"

#define WORD_MAX_LEN 128

struct parser {
    FILE *file;
    char word[WORD_MAX_LEN];
};

struct parser* parser_create(const char* filename) {
    struct parser* parser = malloc(sizeof(struct parser));
    parser->file = fopen(filename, "r");
    if (parser->file == NULL) {
        logger_warn("Parser failed to open %s", filename);
        parser_destroy(parser);
        return NULL;
    }
    return parser;
}
void parser_destroy(struct parser* parser) {
    if (parser->file != NULL) {
        fclose(parser->file);
    }
    free(parser);
}

char* parser_next(struct parser* parser) {
    if (fscanf(parser->file, "%s", parser->word)) {
        return parser->word;
    }
    return NULL;
}