#include <stdio.h>

#include "cjstat/hsperfdata.h"
#include "cjstat/parser.h"
#include "cjstat/util.h"
#include "cjstat/_hashmap.h"

#include "mytest.h"

int main(int argc, char ** argv) {
    signal(SIGQUIT, signal_handler);


    if (argc != 2) {
        fputs("param error\n", stderr);
        return 1;
    }
    int pid = atoi(argv[1]);

    // CASE 1
    hsperfdata * hs = get_hsperfdata_by_pid(pid);
    hsperfdata_entry * p = hs->entry_list;
    while (NULL != p) {
        printf("%s=", (char *)p->key);
        if (perfdata_long == p->data_type) {
            printf("%llu\n", (unsigned long long)(p->value_long));
        } else if (perfdata_byte == p->data_type) {
            printf("\"%s\"\n", p->value_bytearray);
        }

        p = p->next;
    }

    _hashmap * m = convert_hsperfdata_to_hashmap(hs);

    printf("%s = %llu\n", "sun.gc.generation.0.space.0.used", 
            (unsigned long long)(get_hsperfdata_entry_from_hashmap(m,
                        "sun.gc.generation.0.space.0.used")->value_long));
    printf("%s = %s\n", "java.property.java.vm.name",
            get_hsperfdata_entry_from_hashmap(m, "java.property.java.vm.name")->
            value_bytearray);
        
    _hashmap_destroy(m);
    destroy_hsperfdata(hs);

    // CASE 2
    cjstat_jvm_pid * pids = get_all_jvm_pid();
    while (NULL != pids) {
        printf("%d\t%s\n", pids->pid, pids->path);
        get_next_jvm_pid(&pids);
    }

    return 0;
}

