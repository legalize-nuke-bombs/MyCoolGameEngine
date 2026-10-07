#ifndef MYCOOLGAMEENGINE_UINT128_DICTIONARY_H
#define MYCOOLGAMEENGINE_UINT128_DICTIONARY_H

#include "dictionary.h"
#include "../api.h"

// The key is a pointer to an uint128_t and the dictionary compares the numbers, not the pointers:
// the number has to stay in memory for as long as its entry is in the dictionary
MCGE_API struct dictionary* uint128_dictionary_build(int dim);

#endif //MYCOOLGAMEENGINE_UINT128_DICTIONARY_H
