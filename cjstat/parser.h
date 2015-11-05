#ifndef HSPERFDATA_PARSER_H
#define HSPERFDATA_PARSER_H

#include <stdio.h>   /* fprintf, stderr */
#include <string.h>  /* strlen, strcpy */

#include "hsperfdata.h"
#include "_alloc.h"

void __prologue_parse(hsperfdata * pd, byte * base);
void __entries_parse(hsperfdata * pd, byte * base);
hsperfdata_errcode hsperfdata_parse(hsperfdata * pd, byte * base);



void __prologue_parse(hsperfdata * pd, byte * base) {
    hsperfdata_prologue * prologue = pd->prologue;

    prologue -> magic          = *((dword *)(base + 0));
    prologue -> byte_order     = *((byte *) (base + 4));
    prologue -> major_version  = *((byte *) (base + 5));
    prologue -> minor_version  = *((byte *) (base + 6));
    prologue -> accessible     = *((byte *) (base + 7));
    prologue -> used           = *((dword *)(base + 8));
    prologue -> overflow       = *((dword *)(base + 12));
    prologue -> mod_time_stamp = *((qword *)(base + 16));
    prologue -> entry_offset   = *((dword *)(base + 24));
    prologue -> num_entries    = *((dword *)(base + 28));
    return;
}



void __entries_parse(hsperfdata * pd, byte * base) {
    byte * entry_base = base + pd->prologue->entry_offset;
    byte * key_base;
    byte * value_base;
    
    hsperfdata_entry * node;  /* entry node */
    
    for (size_t i=0; i < (size_t)pd->prologue->num_entries; ++i) {
        node = NULL;
        CALLOC_WITH_CHECK(node, sizeof(hsperfdata_entry));
        
        node -> entry_length     = *((dword *)(entry_base + 0));
        node -> name_offset      = *((dword *)(entry_base + 4));
        node -> vector_length    = *((dword *)(entry_base + 8));
        node -> data_type        = *((byte *) (entry_base + 12));
        node -> flags            = *((byte *) (entry_base + 13));
        node -> data_units       = *((byte *) (entry_base + 14));
        node -> data_variability = *((byte *) (entry_base + 15));
        node -> data_offset      = *((dword *)(entry_base + 16));

        /* parse key */
        key_base = entry_base + node->name_offset;
        size_t key_length = strlen((char *)key_base);
        CALLOC_WITH_CHECK(node->key, key_length+1);  /* '\0' */
        strcpy((char *)node->key, (char *)key_base);

        /* parse value */
        value_base = entry_base + node->data_offset;
        if (0 == node->vector_length) {

            if (perfdata_long != (hsperfdata_entry_datatype)(node->data_type)) {
#ifndef NDEBUG
                fprintf(stderr, "unsupported data type found when parsing hsperfdata\n");
#endif  /* NDEBUG */
            }
            node->value_long = *((qword *)value_base);

        } else {

            if (perfdata_byte != (hsperfdata_entry_datatype)(node->data_type)) {
#ifndef NDEBUG
                fprintf(stderr, "unsupported data type found when parsing hsperfdata\n");
#endif  /* NDEBUG */
            }
            size_t value_length = strlen((char *)value_base);
            CALLOC_WITH_CHECK(node->value_bytearray, value_length+1);  /* '\0' */
            strcpy((char *)node->value_bytearray, (char *)value_base);

        }

        /* move pointer on hsperfdata */
        entry_base = entry_base + node->entry_length;

        /* linked list insert */
        node->next = pd->entry_list;
        pd->entry_list = node;
    }
    return;
}



hsperfdata_errcode hsperfdata_parse(hsperfdata * pd, byte * base) {
    assert(NULL != pd);
    assert(NULL != base);

    __prologue_parse(pd, base);

    /* check magic number */
    if (HSPERFDATA_MAGIC != pd->prologue->magic) {
        return incorrect_magic_number;
    }

    /* check endian */
    /* TODO endian support */
    if (HSPERFDATA_BIG_ENDIAN == pd->prologue->byte_order) {
        return not_implemented;
    }

    /* check if accessible */
    if (false == pd->prologue->accessible) {
        return inaccessiable;
    }

    __entries_parse(pd, base);
    return ok;
}

#endif  /* HSPERFDATA_PARSER_H */

