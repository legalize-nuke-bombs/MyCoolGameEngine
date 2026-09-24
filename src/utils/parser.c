//
// Created by nikita on 23.09.2026.
//

#include "parser.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../logging/logger.h"

struct parser {
    char *buffer;
    unsigned long long size;
    unsigned long long cursor;
    const char *pending;
};

struct parser* parser_create(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        logger_warn("Parser failed to open %s", filename);
        return NULL;
    }
    _fseeki64(file, 0, SEEK_END);
    const long long size = _ftelli64(file);
    _fseeki64(file, 0, SEEK_SET);

    struct parser* parser = malloc(sizeof(struct parser));
    parser->buffer = malloc(size + 1);
    parser->size = fread(parser->buffer, 1, size, file);
    parser->buffer[parser->size] = '\0';
    parser->cursor = 0;
    parser->pending = NULL;
    fclose(file);
    return parser;
}

void parser_destroy(struct parser* parser) {
    if (parser != NULL) {
        free(parser->buffer);
        free(parser);
    }
}

const char* parser_next(struct parser* parser) {
    if (parser->pending != NULL) {
        const char *word = parser->pending;
        parser->pending = NULL;
        return word;
    }
    while (parser->cursor < parser->size && isspace((unsigned char)parser->buffer[parser->cursor])) {
        parser->cursor++;
    }
    if (parser->cursor >= parser->size) {
        return NULL;
    }
    const char *word = parser->buffer + parser->cursor;
    while (parser->cursor < parser->size && !isspace((unsigned char)parser->buffer[parser->cursor])) {
        parser->cursor++;
    }
    parser->buffer[parser->cursor++] = '\0';
    return word;
}
char* parser_next_dup(struct parser* parser) {
    const char* word = parser_next(parser);
    return word != NULL ? strdup(word) : NULL;
}

static const char* parser_next_expected(struct parser* parser, const char* expected) {
    const char* word = parser_next(parser);
    if (word == NULL) {
        logger_warn("Parser expected %s, got end of file", expected);
    }
    return word;
}
static int parser_reject(struct parser* parser, const char* expected, const char* word) {
    logger_warn("Parser expected %s, got %s", expected, word);
    parser->pending = word;
    return 0;
}

static int parser_next_long(struct parser* parser, const char* expected, const long min, const long max, long* out_value) {
    *out_value = 0;
    const char* word = parser_next_expected(parser, expected);
    if (word == NULL) {
        return 0;
    }
    char* end;
    errno = 0;
    const long value = strtol(word, &end, 10);
    if (end == word || *end != '\0' || errno == ERANGE || value < min || value > max) {
        return parser_reject(parser, expected, word);
    }
    *out_value = value;
    return 1;
}

int parser_next_int(struct parser* parser, int* out_value) {
    long value;
    const int ok = parser_next_long(parser, "int", INT_MIN, INT_MAX, &value);
    *out_value = (int)value;
    return ok;
}
int parser_next_char(struct parser* parser, char* out_value) {
    *out_value = '\0';
    const char* word = parser_next_expected(parser, "char");
    if (word == NULL) {
        return 0;
    }
    if (word[1] != '\0') {
        return parser_reject(parser, "char", word);
    }
    *out_value = word[0];
    return 1;
}
uint8_t parser_next_uint8(struct parser* parser, uint8_t* out_value) {
    long value;
    const int ok = parser_next_long(parser, "uint8", 0, UINT8_MAX, &value);
    *out_value = (uint8_t)value;
    return ok;
}
int parser_next_double(struct parser* parser, double* out_value) {
    *out_value = 0;
    const char* word = parser_next_expected(parser, "double");
    if (word == NULL) {
        return 0;
    }
    char* end;
    const double value = strtod(word, &end);
    if (end == word || *end != '\0') {
        return parser_reject(parser, "double", word);
    }
    *out_value = value;
    return 1;
}
