#include <stdio.h>
#include <stdbool.h>

#include "cjstat/_hashmap.h"

int main() {
    _hashmap * map = _hashmap_create();

    char key[1000], value[1000];

    while (true) {
        char ch = (char)getchar();
        switch (ch) {
            case 'P':  /* put */
                scanf("%s%s", key, value);
                if (errcode_existed == 
                        _hashmap_put(map, _hashmap_entry_create(key, value, strlen(value)+1))) {
                    puts("existed");
                    fflush(stdout);
                } else {
                    puts("done");
                    fflush(stdout);
                }
                break;
            case 'G':  /* get */
                scanf("%s", key);
                _hashmap_entry * entry = _hashmap_get(map, key);
                if (NULL != entry) {
                    printf("%s\n", (char *)(entry->value));
                    fflush(stdout);
                } else {
                    puts("null");
                    fflush(stdout);
                }
                break;
            case 'C':  /* containsKey */
                scanf("%s", key);
                printf("%d\n", _hashmap_contains_key(map, key) == true ? 1 : 0);
                fflush(stdout);
                break;
            case 'I':  /* is_empty */
                printf("%d\n", _hashmap_is_empty(map) == true ? 1 : 0);
                fflush(stdout);
                break;
            case 'Q':  /* quit */
                goto quit;
                break;
            case '\n':  /* just continue */
                continue;
            default:
                fprintf(stderr, "wrong command \"%d\"\n", ch);
                return 1;
        }
    }

quit:

    _hashmap_destroy(map);
    return 0;
}

