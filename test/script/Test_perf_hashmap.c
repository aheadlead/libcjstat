#include <stdio.h>

#include "cjstat/_hashmap.h"
#include "cjstat/_alloc.h"

#include "perftest.h"

void hashmap_put_n_entrys(unsigned int inventory_num) {
    char (*key)[16] = NULL;
    int * value = NULL;
    CALLOC_WITH_CHECK(key, inventory_num * 16 * sizeof(char));
    CALLOC_WITH_CHECK(value, inventory_num * sizeof(int));
    for (unsigned int i=0; i<inventory_num; ++i) {
        RANDSTRING(key[i], 15);
        value[i] = RANDINT(0, 1000000);
    }

    START_TEST
    PAUSE_TEST
        _hashmap * map = _hashmap_create();
    RESUME_TEST
        for (unsigned int i=0; i<inventory_num; ++i) {
            _hashmap_put(map, 
                    _hashmap_entry_create(
                        key[i], 
                        value+i,
                        sizeof(int)));
        }
    PAUSE_TEST
        _hashmap_destroy(map);
    RESUME_TEST
    STOP_TEST

    SAFE_FREE_CLEAN(key);
    SAFE_FREE_CLEAN(value);
}

int main() {
    RANDINIT();

    int repeat = 500;
    PREPARE_TEST("hashmap put 300 entrys", repeat) {
        hashmap_put_n_entrys(300);
    } SHOW_TEST();
    
    PREPARE_TEST("hashmap put 1000 entrys", repeat) {
        hashmap_put_n_entrys(1000);
    } SHOW_TEST();
    
    return 0;
}

