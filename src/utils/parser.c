//
// Created by nikita on 23.09.2026.
//

#include "parser.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../logging/logger.h"

#define WORD_MAX_LEN 127
#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#define WORD_FORMAT "%" STRINGIFY(WORD_MAX_LEN) "s"

struct parser {
    FILE *file;
    char word[WORD_MAX_LEN + 1];
    bool word_pending;
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
    parser->word_pending = false;
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
    if (parser->word_pending) {
        parser->word_pending = false;
        return parser->word;
    }
    if (fscanf(parser->file, WORD_FORMAT, parser->word) == 1) {
        return parser->word;
    }
    return NULL;
}
char* parser_next_dup(struct parser* parser) {
    const char* word = parser_next(parser);
    return word != NULL ? strdup(word) : NULL;
}

static int parser_next_scanned(struct parser* parser, const char* expected, const char* format, void* out_value) {
    const char* word = parser_next(parser);
    if (word == NULL) {
        logger_warn("Parser expected %s, got end of file", expected);
        return 0;
    }
    int consumed = 0;
    sscanf(word, format, out_value, &consumed);
    if (consumed == (int)strlen(word)) {
        return 1;
    }
    logger_warn("Parser expected %s, got %s", expected, word);
    parser->word_pending = true;
    return 0;
}

int parser_next_int(struct parser* parser, int* out_value) {
    *out_value = 0;
    return parser_next_scanned(parser, "int", "%d%n", out_value);
}
int parser_next_char(struct parser* parser, char* out_value) {
    *out_value = '\0';
    return parser_next_scanned(parser, "char", "%c%n", out_value);
}
uint8_t parser_next_uint8(struct parser* parser, uint8_t* out_value) {
    *out_value = 0;
    return parser_next_scanned(parser, "uint8", "%" SCNu8 "%n", out_value);
}
int parser_next_double(struct parser* parser, double* out_value) {
    *out_value = 0;
    return parser_next_scanned(parser, "double", "%lf%n", out_value);
}
