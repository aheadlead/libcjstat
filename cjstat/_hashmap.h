/* _hashmap.h */

#ifndef _HASHMAP_H
#define _HASHMAP_H 

/* alternative: 331, 673, 1361, 2729, 5471, 10949, 21991 */
#define HASHMAP_SIZE 2729

#include <stddef.h>     /* NULL */
#include <string.h>     /* memset, memcpy, strcmp */
#include <stdbool.h>    /* bool */

#include "_alloc.h"

#ifdef HASHMAP_SHOW_STATISTIC
    int hashmap_put_collision_count = 0;
    int hashmap_get_collision_count = 0;
#endif  /* HASHMAP_STATISTIC */

typedef enum {
    errcode_ok,
    errcode_full,
    errcode_existed,
    errcode_key_error,
} _hashmap_errcode;

typedef unsigned int hash_type;

typedef struct _hashmap_entry {
    hash_type hashed_key;
    size_t length;  /* entry length in bytes */

    char * key;
    void * value;
} _hashmap_entry;

typedef struct _hashmap {
    _hashmap_entry * map[HASHMAP_SIZE];
    unsigned long size;  /* entry number */
} _hashmap;



/* 
 * prototype 
 */
_hashmap *         _hashmap_create        ();
void               _hashmap_init          (_hashmap * map);
void               _hashmap_destroy       (_hashmap * map);

_hashmap_entry *   _hashmap_entry_create  (const char * key,
                                              void * value,
                                              size_t value_length);
void               _hashmap_entry_destroy (_hashmap_entry * entry);

_hashmap_errcode   _hashmap_put           (_hashmap * map, 
                                              _hashmap_entry * entry);
_hashmap_entry *   _hashmap_get           (_hashmap * map, const char * key);
/* _hashmap_errcode   _hashmap_remove        (_hashmap * map, const char * key); */

bool               _hashmap_contains_key  (_hashmap * map, const char * key);
bool               _hashmap_is_empty      (_hashmap * map);

static hash_type          __string_hash          (const char * str);



/* 
 * implementation 
 */
_hashmap * _hashmap_create() {
    _hashmap * ret=NULL;
    CALLOC_WITH_CHECK(
            ret,
            sizeof(_hashmap));
    _hashmap_init(ret);
    return ret;
}


void _hashmap_init(_hashmap * fu) {
    assert(NULL != fu && "attempt to init a null pointer as a _hashmap");
    memset(fu->map, 0x0, HASHMAP_SIZE);
    fu->size = 0;
    return;
}


void _hashmap_destroy(_hashmap * fu) {
    assert(NULL != fu && "attempt to destroy a null pointer as a _hashmap");
    for (_hashmap_entry ** entry_ptr=fu->map;
            entry_ptr < fu->map+HASHMAP_SIZE;
            ++entry_ptr) {
        if (NULL != *entry_ptr) {
            _hashmap_entry_destroy(*entry_ptr);
            *entry_ptr = NULL;
        }
    }
    SAFE_FREE_CLEAN(fu);
    return;
}


_hashmap_entry * _hashmap_entry_create(
        const char * key, void * value, size_t value_length) {
    _hashmap_entry * ret = NULL;
    CALLOC_WITH_CHECK(ret, sizeof(_hashmap_entry));
    
    /* key */
    unsigned long key_length = strlen(key);
    CALLOC_WITH_CHECK(ret->key, sizeof(char)*key_length+1);
    memcpy(ret->key, key, key_length+1);
    
    /* value & length */
    ret->length = value_length;
    CALLOC_WITH_CHECK(ret->value, value_length);
    memcpy(ret->value, value, value_length);

    return ret;
}


void _hashmap_entry_destroy(_hashmap_entry * entry) {
    assert(entry != NULL && "attempt to destroy a null pointer as a _hashmap_entry");
    SAFE_FREE_CLEAN(entry->key);
    SAFE_FREE_CLEAN(entry->value);
    SAFE_FREE_CLEAN(entry);
    return;
}


_hashmap_errcode _hashmap_put(_hashmap * m, _hashmap_entry * entry) {
    hash_type hash_value=__string_hash(entry->key);
    entry->hashed_key = hash_value;

    size_t index=hash_value % HASHMAP_SIZE;
    const size_t end_index=index+HASHMAP_SIZE;
    
    for (; ; ) {
        if (index == end_index) {
            return errcode_full;
        }
        _hashmap_entry ** entry_ptr = &(m->map[index % HASHMAP_SIZE]);
        if (*entry_ptr != NULL) {  /* any entry exists on entry_ptr ? */
#ifdef HASHMAP_SHOW_STATISTIC
            hashmap_put_collision_count += 1;
#endif  /* HASHMAP_SHOW_STATISTIC */
            if ((*entry_ptr)->hashed_key == hash_value &&  
                    /* has same hash value ? */
                    0 == strcmp((*entry_ptr)->key, entry->key)) 
                    /* compare key strictly */ {  
                return errcode_existed;
            }
        } else {
            *entry_ptr = entry;  /* insert */
            m->size += 1;
#ifndef NDEBUG
            assert(m->size <= HASHMAP_SIZE);
#endif  /* NDEBUG */
            break;
        }
        index += 1;
    }
    return errcode_ok;
}


_hashmap_entry * _hashmap_get(_hashmap * m, const char * key) {
    hash_type hash_value=__string_hash(key);
    size_t index=hash_value % HASHMAP_SIZE;
    const size_t end_index=index+HASHMAP_SIZE;

    for (; ; ) {
        if (index == end_index) {
            return NULL;  /* end in entry not found */
        }
        _hashmap_entry ** entry_ptr = &(m->map[index % HASHMAP_SIZE]);
        if (*entry_ptr != NULL) {  /* any entry exists on entry_ptr ? */
#ifdef HASHMAP_SHOW_STATISTIC
            hashmap_get_collision_count += 1;
#endif  /* HASHMAP_SHOW_STATISTIC */
            if ((*entry_ptr)->hashed_key == hash_value &&  
                    /* has same hash value ? */
                    0 == strcmp((*entry_ptr)->key, key)) 
                    /* compare key strictly */ {  
                return *entry_ptr;
            }
        } else {
            return NULL;
        }
        index += 1;
    }
}


bool _hashmap_contains_key(_hashmap * m, const char * key) {
    hash_type hash_value=__string_hash(key);
    size_t index=hash_value % HASHMAP_SIZE;
    const size_t end_index=index+HASHMAP_SIZE;

    for (; ; ) {
        if (index == end_index) {
            return false;  /* end in entry not found */
        }
        _hashmap_entry ** entry_ptr = &(m->map[index % HASHMAP_SIZE]);
        if (*entry_ptr != NULL) {  /* any entry exists on entry_ptr ? */
#ifdef HASHMAP_SHOW_STATISTIC
            hashmap_get_collision_count += 1;
#endif  /* HASHMAP_SHOW_STATISTIC */
            if ((*entry_ptr)->hashed_key == hash_value &&  
                    /* has same hash value ? */
                    0 == strcmp((*entry_ptr)->key, key)) 
                    /* compare key strictly */ {  
                return true;
            }
        } else {
            return false;
        }
        index += 1;
    }
}


inline bool _hashmap_is_empty(_hashmap * m) {
    return 0 == m->size;
}


static hash_type __string_hash(const char * str) {
    /* BKDRHash */
    hash_type hash=0;
    while (*str)
        hash = hash*0x83 + (hash_type)(unsigned char)(*str++);
    return hash & 0x7FFFFFFFL;
}

#endif // _HASHMAP_H

