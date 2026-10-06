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
// The holder gives up its ref only after the destructor: the destructor may destroy the last refs to this object,
// and the block must outlive that. The refs see NULL from the first moment of the destructor.
void holder_destroy(holder *this) {
    struct control_block *block = this->_block;
    this->_block = NULL;
    block->holders--;
    if (block->holders == 0) {
        void *ptr = block->ptr;
        block->ptr = NULL;
        block->destructor(ptr);
    }
    block->refs--;
    if (block->refs == 0) {
        free(block);
    }
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