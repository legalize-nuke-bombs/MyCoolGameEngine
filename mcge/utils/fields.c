#include "fields.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "../logging/logger.h"




struct fields_list {
    struct list nodes;
};

struct fields_field {
    char *name;
    struct fields_list value;
    bool asked;
};

struct fields {
    char *key;
    int line;
    bool is_number;
    double number;
    struct list fields;
};


enum token_type {
    token_word,
    token_equals,
    token_list_start,
    token_list_end,
    token_end
};

struct token {
    enum token_type type;
    const char *start;
    int length;
    int line;
};

struct reader {
    const char *filename;
    const char *cursor;
    int line;
    bool failed;

    struct token current;
    struct token next;
};


static void reader_fail(struct reader *this, const int line, const char *message) {
    if (!this->failed) {
        logger_error("%s:%d: %s", this->filename, line, message);
    }
    this->failed = true;
}

static void reader_skip_blank(struct reader *this) {
    for (;;) {
        const char c = *this->cursor;
        if (c == '\n') {
            this->line++;
            this->cursor++;
        }
        else if (isspace((unsigned char)c)) {
            this->cursor++;
        }
        else if (c == '/' && this->cursor[1] == '*') {
            const int line = this->line;
            this->cursor += 2;
            while (*this->cursor != '\0' && !(this->cursor[0] == '*' && this->cursor[1] == '/')) {
                if (*this->cursor == '\n') {
                    this->line++;
                }
                this->cursor++;
            }
            if (*this->cursor == '\0') {
                reader_fail(this, line, "comment is not closed");
                return;
            }
            this->cursor += 2;
        }
        else {
            return;
        }
    }
}

static bool is_word_char(const char c) {
    return c != '\0' && !isspace((unsigned char)c) && c != '=' && c != '[' && c != ']' && c != '"';
}

static struct token reader_read_token(struct reader *this) {
    reader_skip_blank(this);

    struct token token = {
        .type = token_end,
        .start = this->cursor,
        .length = 0,
        .line = this->line
    };
    if (this->failed || *this->cursor == '\0') {
        return token;
    }

    const char c = *this->cursor;
    if (c == '=' || c == '[' || c == ']') {
        token.type = c == '=' ? token_equals : c == '[' ? token_list_start : token_list_end;
        token.length = 1;
        this->cursor++;
        return token;
    }

    if (c == '"') {
        this->cursor++;
        token.start = this->cursor;
        while (*this->cursor != '\0' && *this->cursor != '"') {
            if (*this->cursor == '\n') {
                this->line++;
            }
            this->cursor++;
        }
        if (*this->cursor == '\0') {
            reader_fail(this, token.line, "string is not closed");
            return token;
        }
        token.type = token_word;
        token.length = (int)(this->cursor - token.start);
        this->cursor++;
        return token;
    }

    token.type = token_word;
    while (is_word_char(*this->cursor)) {
        this->cursor++;
    }
    token.length = (int)(this->cursor - token.start);
    return token;
}

static void reader_advance(struct reader *this) {
    this->current = this->next;
    this->next = reader_read_token(this);
}

static char* token_dup(const struct token token) {
    char *word = malloc(token.length + 1);
    memcpy(word, token.start, token.length);
    word[token.length] = '\0';
    return word;
}


static bool word_to_double(const char *word, double *out_value) {
    char *end;
    const double value = strtod(word, &end);
    if (end == word || *end != '\0') {
        return false;
    }
    *out_value = value;
    return true;
}

static struct fields* fields_create(const struct token token) {
    struct fields *this = calloc(1, sizeof(struct fields));
    this->key = token_dup(token);
    this->line = token.line;
    this->is_number = word_to_double(this->key, &this->number);
    this->fields = list_create(1);
    return this;
}

static void fields_list_clear(struct fields_list *this) {
    for (int i = 0; i < list_count(&this->nodes); i++) {
        fields_destroy(list_get(&this->nodes, i));
    }
    list_destroy(&this->nodes);
}

static void fields_field_destroy(struct fields_field *this) {
    fields_list_clear(&this->value);
    free(this->name);
    free(this);
}

void fields_destroy(struct fields *this) {
    for (int i = 0; i < list_count(&this->fields); i++) {
        fields_field_destroy(list_get(&this->fields, i));
    }
    list_destroy(&this->fields);
    free(this->key);
    free(this);
}

static struct fields_field* fields_find(const struct fields *this, const char *name) {
    for (int i = 0; i < list_count(&this->fields); i++) {
        struct fields_field *field = list_get(&this->fields, i);
        if (strcmp(field->name, name) == 0) {
            return field;
        }
    }
    return NULL;
}


static struct fields* reader_parse_node(struct reader *this);

static bool reader_parse_value(struct reader *this, struct fields_list *out_value) {
    if (this->current.type == token_word && this->next.type != token_equals) {
        list_add(&out_value->nodes, fields_create(this->current));
        reader_advance(this);
        return true;
    }

    if (this->current.type != token_list_start) {
        reader_fail(this, this->current.line, "expected value after `=`");
        return false;
    }
    const int line = this->current.line;
    reader_advance(this);

    while (this->current.type == token_word) {
        struct fields *node = reader_parse_node(this);
        if (node == NULL) {
            return false;
        }
        list_add(&out_value->nodes, node);
    }
    if (this->current.type != token_list_end) {
        reader_fail(this, this->current.type == token_end ? line : this->current.line, this->current.type == token_end ? "list is not closed" : "expected word or `]` inside list");
        return false;
    }
    reader_advance(this);
    return true;
}

static struct fields* reader_parse_node(struct reader *this) {
    struct fields *node = fields_create(this->current);
    reader_advance(this);

    while (this->current.type == token_word && this->next.type == token_equals) {
        struct fields_field *field = calloc(1, sizeof(struct fields_field));
        field->name = token_dup(this->current);
        field->value.nodes = list_create(1);
        const int line = this->current.line;
        reader_advance(this);
        reader_advance(this);

        if (!reader_parse_value(this, &field->value)) {
            fields_field_destroy(field);
            fields_destroy(node);
            return NULL;
        }
        if (fields_find(node, field->name) != NULL) {
            logger_warn("%s:%d: %s already has field `%s`, the second one is ignored", this->filename, line, node->key, field->name);
            fields_field_destroy(field);
            continue;
        }
        list_add(&node->fields, field);
    }

    return node;
}

static char* read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    buffer[fread(buffer, 1, size, file)] = '\0';
    fclose(file);
    return buffer;
}

struct fields_list* fields_parse_file(const char *filename) {
    char *buffer = read_file(filename);
    if (buffer == NULL) {
        logger_warn("Fields failed to open %s", filename);
        return NULL;
    }

    struct reader reader = {
        .filename = filename,
        .cursor = buffer,
        .line = 1
    };
    reader_advance(&reader);
    reader_advance(&reader);

    struct fields_list *list = calloc(1, sizeof(struct fields_list));
    list->nodes = list_create(8);

    while (reader.current.type == token_word) {
        struct fields *node = reader_parse_node(&reader);
        if (node == NULL) {
            break;
        }
        list_add(&list->nodes, node);
    }
    if (!reader.failed && reader.current.type != token_end) {
        reader_fail(&reader, reader.current.line, "expected word");
    }

    free(buffer);
    if (reader.failed) {
        fields_list_destroy(list);
        return NULL;
    }
    return list;
}

void fields_list_destroy(struct fields_list *this) {
    fields_list_clear(this);
    free(this);
}

int fields_list_count(const struct fields_list *this) {
    return this != NULL ? list_count(&this->nodes) : 0;
}
struct fields* fields_list_get(const struct fields_list *this, const int index) {
    return list_get(&this->nodes, index);
}


const char* fields_key(const struct fields *this) {
    return this->key;
}
int fields_line(const struct fields *this) {
    return this->line;
}

bool fields_has(const struct fields *this, const char *name) {
    return fields_find(this, name) != NULL;
}

static const struct fields* fields_get_value(const struct fields *this, const char *name) {
    struct fields_field *field = fields_find(this, name);
    if (field == NULL) {
        return NULL;
    }
    field->asked = true;
    if (list_count(&field->value.nodes) != 1) {
        logger_warn("%s (line %d) expected one value in field `%s`, got %d", this->key, this->line, name, list_count(&field->value.nodes));
        return NULL;
    }
    return list_get(&field->value.nodes, 0);
}

const char* fields_get_string(struct fields *this, const char *name, const char *default_value) {
    const struct fields *value = fields_get_value(this, name);
    return value != NULL ? value->key : default_value;
}
char* fields_dup_string(struct fields *this, const char *name, const char *default_value) {
    const char *word = fields_get_string(this, name, default_value);
    return word != NULL ? strdup(word) : NULL;
}

bool fields_get_bool(struct fields *this, const char *name, const bool default_value) {
    const struct fields *value = fields_get_value(this, name);
    if (value == NULL) {
        return default_value;
    }
    if (strcmp(value->key, "true") == 0 || strcmp(value->key, "1") == 0) {
        return true;
    }
    if (strcmp(value->key, "false") == 0 || strcmp(value->key, "0") == 0) {
        return false;
    }
    logger_warn("%s (line %d) expected true or false in field `%s`, got %s", this->key, this->line, name, value->key);
    return default_value;
}

double fields_get_double(struct fields *this, const char *name, const double default_value) {
    const struct fields *value = fields_get_value(this, name);
    if (value == NULL) {
        return default_value;
    }
    if (!value->is_number) {
        logger_warn("%s (line %d) expected double in field `%s`, got %s", this->key, this->line, name, value->key);
        return default_value;
    }
    return value->number;
}

int fields_get_int(struct fields *this, const char *name, const int default_value) {
    const char *word = fields_get_string(this, name, NULL);
    if (word == NULL) {
        return default_value;
    }
    char *end;
    errno = 0;
    const long value = strtol(word, &end, 10);
    if (end == word || *end != '\0' || errno == ERANGE || value < INT_MIN || value > INT_MAX) {
        logger_warn("%s (line %d) expected int in field `%s`, got %s", this->key, this->line, name, word);
        return default_value;
    }
    return (int)value;
}

struct vector2 fields_get_vector2(struct fields *this, const char *name, const struct vector2 default_value) {
    const struct fields_list *list = fields_get_list(this, name);
    if (list == NULL) {
        return default_value;
    }
    if (fields_list_count(list) != 2 || !fields_list_get(list, 0)->is_number || !fields_list_get(list, 1)->is_number) {
        logger_warn("%s (line %d) expected two doubles in field `%s`", this->key, this->line, name);
        return default_value;
    }
    const struct vector2 value = {
        .x = fields_list_get(list, 0)->number,
        .y = fields_list_get(list, 1)->number
    };
    return value;
}

struct color fields_get_color(struct fields *this, const char *name, const struct color default_value) {
    const struct fields_list *list = fields_get_list(this, name);
    if (list == NULL) {
        return default_value;
    }
    uint8_t channels[4];
    bool ok = fields_list_count(list) == 4;
    for (int i = 0; ok && i < 4; i++) {
        const struct fields *channel = fields_list_get(list, i);
        ok = channel->is_number && channel->number >= 0 && channel->number <= UINT8_MAX && channel->number == (int)channel->number;
        channels[i] = ok ? (uint8_t)channel->number : 0;
    }
    if (!ok) {
        logger_warn("%s (line %d) expected four numbers from 0 to 255 in field `%s`", this->key, this->line, name);
        return default_value;
    }
    const struct color value = {
        .r = channels[0],
        .g = channels[1],
        .b = channels[2],
        .a = channels[3]
    };
    return value;
}

const struct fields_list* fields_get_list(struct fields *this, const char *name) {
    struct fields_field *field = fields_find(this, name);
    if (field == NULL) {
        return NULL;
    }
    field->asked = true;
    return &field->value;
}

void fields_warn_unknown(struct fields *this) {
    for (int i = 0; i < list_count(&this->fields); i++) {
        struct fields_field *field = list_get(&this->fields, i);
        if (!field->asked) {
            logger_warn("%s (line %d) does not know field `%s`", this->key, this->line, field->name);
            field->asked = true;
        }
    }
}

struct fields* fields_clone(const struct fields *this) {
    struct fields *clone = calloc(1, sizeof(struct fields));
    clone->key = strdup(this->key);
    clone->line = this->line;
    clone->is_number = this->is_number;
    clone->number = this->number;
    clone->fields = list_create(list_count(&this->fields));
    for (int i = 0; i < list_count(&this->fields); i++) {
        const struct fields_field *field = list_get(&this->fields, i);
        struct fields_field *field_clone = calloc(1, sizeof(struct fields_field));
        field_clone->name = strdup(field->name);
        field_clone->asked = field->asked;
        field_clone->value.nodes = list_create(list_count(&field->value.nodes));
        for (int j = 0; j < list_count(&field->value.nodes); j++) {
            list_add(&field_clone->value.nodes, fields_clone(list_get(&field->value.nodes, j)));
        }
        list_add(&clone->fields, field_clone);
    }
    return clone;
}
