//
// Created by Nikita on 06.10.2026.
//

#include "ref.h"

#include <stdlib.h>


void ref_destroy(ref *this) {
    this->_block->refs--;
    if (this->_block->refs == 0) {
        free(this->_block);
    }
    this->_block = NULL;
}

ref ref_copy(const ref *ref) {
    const struct ref this = *ref;
    this._block->refs++;
    return this;
}

void* ref_ptr(const ref *this) {
    return this->_block->ptr;
}