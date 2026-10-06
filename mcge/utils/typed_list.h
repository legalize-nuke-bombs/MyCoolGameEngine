// Example:
// #define LIST_NAME action_method_list
// #define LIST_TYPE struct action_method
// #include "typed_list.h"

#include <stdlib.h>

#define TYPED_LIST_CONCAT_(a, b) a##b
#define TYPED_LIST_CONCAT(a, b) TYPED_LIST_CONCAT_(a, b)
#define TYPED_LIST_FUNCTION(name) TYPED_LIST_CONCAT(LIST_NAME, TYPED_LIST_CONCAT(_, name))

struct LIST_NAME {
    LIST_TYPE *_data;
    int _capacity;
    int _count;
};

static struct LIST_NAME TYPED_LIST_FUNCTION(create)(int capacity) {
    if (capacity <= 0) {
        capacity = 1;
    }
    const struct LIST_NAME list = {
        ._data = NULL,
        ._capacity = capacity,
        ._count = 0
    };
    return list;
}
static void TYPED_LIST_FUNCTION(destroy)(struct LIST_NAME *this) {
    free(this->_data);
}

static int TYPED_LIST_FUNCTION(count)(const struct LIST_NAME *this) {
    return this->_count;
}

static LIST_TYPE *TYPED_LIST_FUNCTION(get)(const struct LIST_NAME *this, const int index) {
    return &this->_data[index];
}

static void TYPED_LIST_FUNCTION(add)(struct LIST_NAME *this, const LIST_TYPE value) {
    if (this->_data == NULL) {
        if (this->_capacity < 1) {
            this->_capacity = 1;
        }
        this->_data = malloc(sizeof(LIST_TYPE) * this->_capacity);
    }
    else if (this->_count >= this->_capacity) {
        this->_capacity *= 2;
        this->_data = realloc(this->_data, sizeof(LIST_TYPE) * this->_capacity);
    }
    this->_data[this->_count++] = value;
}

static void TYPED_LIST_FUNCTION(clear)(struct LIST_NAME *this) {
    this->_count = 0;
}

#undef TYPED_LIST_FUNCTION
#undef TYPED_LIST_CONCAT
#undef TYPED_LIST_CONCAT_
#undef LIST_TYPE
#undef LIST_NAME
