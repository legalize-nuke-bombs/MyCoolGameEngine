//
// Created by Nikita on 06.10.2026.
//

#include "holder.h"

#include <stdlib.h>


holder holder_create(void *ptr, void (*destructor)(void *ptr)) {
    holder this;
    this._block = calloc(1, sizeof(struct control_block));
    this._block->ptr = ptr;
    this._block->destructor = destructor;
    this._block->holders = 1;
    this._block->refs = 1;
    return this;
}
void holder_destroy(holder *this) {
    this->_block->holders--;
    this->_block->refs--;
    if (this->_block->holders == 0) {
        this->_block->destructor(this->_block->ptr);
        this->_block->ptr = NULL;
    }
    if (this->_block->refs == 0) {
        free(this->_block);
    }
    this->_block = NULL;
}

holder holder_copy(const holder *holder) {
    const struct holder this = *holder;
    this._block->holders++;
    this._block->refs++;
    return this;
}
ref holder_ref(const holder *holder) {
    ref ref;
    ref._block = holder->_block;
    ref._block->refs++;
    return ref;
}

void* holder_ptr(const holder *this) {
    return this->_block->ptr;
}