#ifndef HSPERFDATA_H
#define HSPERFDATA_H

/* platform check */
#if defined(__APPLE__) && defined(__MACH__)  /* os x */
#elif defined(__linux__)  /* linux */
#else
    #error "No support for current platform."
#endif

#include <assert.h>   /* assert */
#include <stdint.h>   /* uint8_t, uint16_t, uint32_t, uint64_t */
#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* true, false */

#include "_alloc.h"

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

typedef size_t byte_n;

typedef enum {
    ok,
    incorrect_magic_number,
    inaccessiable,
    not_implemented,
    unsupported_data_type
} hsperfdata_errcode;

typedef enum {
    /* In perfMemory.hpp:87, more than two data types are defined.
     * I havn't seen other types appeared in real JVM perfdata except 'J' and
     * 'B', hence I implemented 'J' and 'B' only.
     */
    perfdata_long = 'J',
    perfdata_byte = 'B'
} hsperfdata_entry_datatype;

typedef struct hsperfdata_prologue {
    dword   magic;                          /* magic number - 0xcafec0c0 */
    byte    byte_order;                     /* byte order of the buffer */
    byte    major_version;                  /* major and minor version numbers */
    byte    minor_version;
    byte    accessible;                     /* ready to access */
    dword   used;                           /* number of PerfData memory bytes used */
    dword   overflow;                       /* number of bytes of overflow */
    qword   mod_time_stamp;                 /* time stamp of last structural modification */
    dword   entry_offset;                   /* offset of the first PerfDataEntry */
    dword   num_entries;                    /* number of allocated PerfData entries */
} hsperfdata_prologue;

#define HSPERFDATA_MAGIC                    0xc0c0feca
#define HSPERFDATA_BIG_ENDIAN               0x0
#define HSPERFDATA_LITTLE_ENDIAN            0x1

typedef struct hsperfdata_entry {
    struct hsperfdata_entry * next;

    /* header */
    dword   entry_length;                   /* entry length in bytes */
    dword   name_offset;                    /* offset of the data item name */
    dword   vector_length;                  /* length of the vector. If 0, then scalar */
    byte    data_type;                      /* type of the data item - */
                                            /* 'B','Z','J','I','S','C','D','F','V','L','[' */
    byte    flags;                          /* flags indicating misc attributes */
    byte    data_units;                     /* unit of measure for the data type */
    byte    data_variability;               /* variability classification of data type */
    dword   data_offset;                    /* offset of the data item */

    /* key */
    byte *  key;

    /* value  */
    hsperfdata_entry_datatype type; 
    /* only one of them would be used */
    qword   value_long;                      
    byte *  value_bytearray;
    
} hsperfdata_entry;

typedef struct hsperfdata {
    hsperfdata_prologue * prologue;
    hsperfdata_entry * entry_list;
} hsperfdata;

hsperfdata * create_hsperfdata() {
    hsperfdata * ret = NULL;
    CALLOC_WITH_CHECK(ret, sizeof(hsperfdata));

    ret->prologue = NULL;
    CALLOC_WITH_CHECK(ret->prologue, sizeof(hsperfdata_prologue));

    ret->entry_list = NULL;
    return ret;
}

void destroy_hsperfdata(hsperfdata * fu) {
    assert(NULL != fu && "attempt to destroy a null pointer");

    SAFE_FREE_CLEAN(fu->prologue);

    hsperfdata_entry * ptr = fu->entry_list;
    while (NULL != ptr) {
        hsperfdata_entry * next_ptr = ptr->next;

        SAFE_FREE_CLEAN(ptr->key);
        SAFE_FREE_CLEAN(ptr->value_bytearray);

        SAFE_FREE_CLEAN(ptr);
        ptr = next_ptr;
    }

    SAFE_FREE_CLEAN(fu);

    return;
}

#endif  /* HSPERFDATA_H */

