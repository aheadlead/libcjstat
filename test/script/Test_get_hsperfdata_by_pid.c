#include <stdio.h>

#include "cjstat/hsperfdata.h"
#include "cjstat/parser.h"
#include "cjstat/util.h"

int main(int argc, char ** argv) {
    hsperfdata * pd = create_hsperfdata();

    if (argc != 2) {
        fputs("param error\n", stderr);
        return 1;
    }

    int pid = atoi(argv[1]);

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

    destroy_hsperfdata(pd);

    return 0;
}

