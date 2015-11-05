#include <stdio.h>

#include "cjstat/hsperfdata.h"
#include "cjstat/parser.h"
#include "cjstat/util.h"

int main(int argc, char ** argv) {
    if (argc != 2) {
        fputs("param error\n", stderr);
        return 1;
    }

    char * hsperfdata_path = argv[1];

    FILE * f = fopen(hsperfdata_path, "rb");
    byte d[32768];

    fread(d, 32768, 1, f);
    fclose(f);

    hsperfdata * hs = create_hsperfdata();
    
    hsperfdata_parse(hs, d);

    hsperfdata_entry * p = hs->entry_list;
    while (NULL != p) {
        printf("%s=", (char *)p->key);
        if (perfdata_long == p->data_type) {
            printf("%llu\n", (unsigned long long)p->value_long);
        } else if (perfdata_byte == p->data_type) {
            printf("\"%s\"\n", p->value_bytearray);
        }

        p = p->next;
    }


    destroy_hsperfdata(hs);
    return 0;
}

