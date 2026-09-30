#include "interpreter_eval.h"

#include <stdlib.h>

#include "../interpreter_command.h"
#include "../interpreter_command_internal.h"
#include "../../subsystems/subsystem_collection.h"
#include "../../utils/parser.h"
#include "../interpreter.h"


struct interpreter_eval {
    struct interpreter_command base;
};

static const char* interpreter_eval_get_key(const struct interpreter_command *this) {
    return "eval";
}

static void interpreter_eval_execute(const struct interpreter_command *this, struct parser *parser, const struct subsystem_collection *subsystems) {
    const struct interpreter* interpreter = (struct interpreter*)subsystem_collection_get(subsystems, "interpreter");
    const char* rpath = parser_next(parser);
    interpreter_eval(interpreter, rpath);
}

static const struct interpreter_command_vtable eval_vtable = {
    .key = interpreter_eval_get_key,
    .execute = interpreter_eval_execute
};

struct interpreter_eval* interpreter_eval_create() {
    struct interpreter_eval *this = calloc(1, sizeof(struct interpreter_eval));
    this->base.vtable = &eval_vtable;
    return this;
}

struct interpreter_command* interpreter_eval_as_interpreter_command(struct interpreter_eval* this) {
    return (struct interpreter_command*)this;
}