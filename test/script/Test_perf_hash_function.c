#include <stdio.h>

#include "cjstat/_hashmap.h"
#include "cjstat/_alloc.h"

#include "perftest.h"

int main() {
    RANDINIT();

    PREPARE_TEST("function __string_hash for 64K x 1024B string", 5) {
        char (*raw)[1024] = NULL;
        CALLOC_WITH_CHECK(raw, 65536 * 1024 * sizeof(char));  /* 64 KiB */

        for (int i=0; i<65536; ++i) {
            RANDSTRING(raw[i], 1023);
        }

        START_TEST
            for (int i=0; i<65536; ++i) {
                __string_hash(raw[i]);
            }
        STOP_TEST

        SAFE_FREE_CLEAN(raw);
    } SHOW_TEST();

    PREPARE_TEST("once function __string_hash", 100000) {
        char *raw = NULL;
        CALLOC_WITH_CHECK(raw, 1024 * sizeof(char));

        RANDSTRING(raw, 1023);

        START_TEST
            __string_hash(raw);
        STOP_TEST

        SAFE_FREE_CLEAN(raw);
    } SHOW_TEST();

    return 0;
}

