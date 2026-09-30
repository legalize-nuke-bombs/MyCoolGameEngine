#include "path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if defined(_WIN32)
    #define PATH_SEP '\\'
    #define strdup _strdup
#else
    #define PATH_SEP '/'
#endif


char* path_alloc_combined(const char* p1, const char* p2) {
    if (!p1 && !p2) return NULL;
    if (!p1) return strdup(p2);
    if (!p2) return strdup(p1);

    const size_t len1 = strlen(p1);
    const size_t len2 = strlen(p2);

    if (len1 == 0) return strdup(p2);
    if (len2 == 0) return strdup(p1);

    const int has_sep1 = (p1[len1 - 1] == '/' || p1[len1 - 1] == '\\');
    const int has_sep2 = (p2[0] == '/' || p2[0] == '\\');

    char* result = malloc(len1 + len2 + 2);
    if (!result) return NULL;

    if (has_sep1 && has_sep2) {
        sprintf(result, "%s%s", p1, p2 + 1);
    }
    else if (!has_sep1 && !has_sep2) {
        sprintf(result, "%s%c%s", p1, PATH_SEP, p2);
    }
    else {
        sprintf(result, "%s%s", p1, p2);
    }

    return result;
}
