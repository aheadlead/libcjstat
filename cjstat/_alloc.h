#ifndef HSPERFDATA__ALLOC_H
#define HSPERFDATA__ALLOC_H

#include <stdio.h>   /* fprintf */
#include <stdlib.h>  /* free, calloc */
#include <stddef.h>  /* NULL */
#include <assert.h>  /* assert */

/* memory allocation macro */
#define ALLOC_BEFORE_CHECK(p)                                               \
    assert(NULL == (p) && "allocating memory with a non null pointer")
#define ALLOC_AFTER_CHECK(p)                                                \
    assert(NULL != (p) && "allocating memory but out of memory")

/* disabled uninitialized variable check here temporarily 
 *
 * A typical usage of macro CALLOC_WITH_CHECK:
 *
 *  char * str;  // <-- WARNING on this line
 *  CALLOC_WITH_CHECK(str, sizeof(char)*1000);
 *
 * GCC warns on the first line because of uninitialized variable with default
 * params.
 *
 * A better way is assign NULL to pointer when declaring.
 */
#define CALLOC_WITH_CHECK(pointer, len)                                     \
    do {                                                                    \
        _Pragma("GCC diagnostic push")                                      \
        _Pragma("GCC diagnostic ignored \"-Wuninitialized\"")               \
            ALLOC_BEFORE_CHECK(pointer);                                    \
            pointer = (typeof(pointer))calloc(1, (len));                    \
            ALLOC_AFTER_CHECK(pointer);                                     \
        _Pragma("GCC diagnostic pop")                                       \
    } while(0)

#ifdef STOP_WHEN_FREE_NULL_POINTER
#define SAFE_FREE_CLEAN(pointer)                                            \
    do {                                                                    \
        assert(NULL != (pointer) && "try to free a null pointer");          \
        free(pointer);                                                      \
        pointer = NULL;                                                     \
    } while(0)
#else
#define SAFE_FREE_CLEAN(pointer)                                            \
    do {                                                                    \
        if (NULL != (pointer)) {                                            \
            free(pointer);                                                  \
            pointer = NULL;                                                 \
        }                                                                   \
    } while(0)
#endif  /* NDEBUG */


#endif  /* HSPERFDATA__ALLOC_H */

