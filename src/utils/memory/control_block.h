//
// Created by Nikita on 06.10.2026.
//

#ifndef MYCOOLGAMEENGINE_CONTROL_BLOCK_H
#define MYCOOLGAMEENGINE_CONTROL_BLOCK_H

struct control_block {
    void* ptr;
    void (*destructor)(void *ptr);
    unsigned int holders;
    unsigned int refs;
};



#endif //MYCOOLGAMEENGINE_CONTROL_BLOCK_H
