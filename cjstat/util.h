#ifndef HSPERFDATA_UTIL_H
#define HSPERFDATA_UTIL_H

#include <stdio.h>                  /* fprintf, stderr */
#include <stdlib.h>                 /* getenv, atoi */
#include <stddef.h>                 /* NULL */
#include <stdbool.h>                /* bool */
#include <string.h>                 /* strlen, strcpy */
#include <ctype.h>                  /* isdigit */

#include <sys/types.h>              /* off_t */
#include <sys/stat.h>               /* struct stat, stat */

#if defined(__APPLE__) && defined(__MACH__)  /* os x */
    #include <sys/syslimits.h>      /* PATH_MAX */
#elif defined(__linux__)  /* linux */
    #include <linux/limits.h>       /* PATH_MAX */
#endif
#include <dirent.h>                 /* DIR, dirent, opendir */

#include "_alloc.h"
#include "_hashmap.h"
#include "hsperfdata.h"
#include "parser.h"

#ifndef NDEBUG
    #define INLINE
#else
    #define INLINE inline
#endif  /* NDEBUG */


/*************
 * prototype *
 *************/
typedef                 struct cjstat_jvm_pid       cjstat_jvm_pid;

cjstat_jvm_pid *        get_all_jvm_pid             ();
int                     get_next_jvm_pid            (cjstat_jvm_pid ** p);
bool                    __str_startswith            (const char * str, 
                                                     const char * prefix);

typedef                 _hashmap                    hsperfdata_hashmap;
hsperfdata_hashmap *    convert_hsperfdata_to_hashmap   
                                                    (hsperfdata * hs);
hsperfdata_entry *      get_hsperfdata_entry_from_hashmap   
                                                    (hsperfdata_hashmap * hm,
                                                     const char * key);


char *                  get_hsperfdata_path_by_pid  (int pid);

hsperfdata *            get_hsperfdata_by_pid       (int pid);
hsperfdata *            get_hsperfdata_by_path      (char * hsperfdata_path);

int                     __selector                  (const struct dirent *b);
int                     __selector_2                (const struct dirent *b);

/* end of prototype */



/******************
 * implementation *
 ******************/

/* a linked list storing all pids */
struct cjstat_jvm_pid {
    int pid;
    char * path;
    cjstat_jvm_pid * next;
};



/*
 * Returns a linked list storing pids of all running jvms.
 */
INLINE cjstat_jvm_pid * get_all_jvm_pid() {
    cjstat_jvm_pid * ret = NULL;

    /* get temporary directory path */
#if defined(__APPLE__) && defined(__MACH__)  /* OS X */
    char * tmpdir_path = getenv("TMPDIR");  /* no need to free */
    if (NULL == tmpdir_path) {
    #ifndef NDEBUG
        fprintf(stderr, "got null pointer when getenv(\"TMPDIR\")\n");
    #endif  /* NDEBUG */
        return NULL;
    }
#elif defined(__linux__)  /* linux */
    char * tmpdir_path = "/tmp/";
#endif

    unsigned long tmpdir_path_length = strlen(tmpdir_path);
    
    struct dirent ** entrys;
    int n = scandir(tmpdir_path, &entrys, __selector, alphasort);
#ifndef NDEBUG
    if (n == -1) {
        perror("scandir");
    }
#endif  /* NDEBUG */
    
    while (n --> 0) {
        struct dirent * entry = entrys[n];

        /* e.g. entry->d_name : "hsperfdata_yulan.wyl" ...
         *      (sampled from my macintosh)
         */
         
        /* make hsperfdata dir path */
        char * hsperfdata_dir_path = NULL;
        /* end with slash ? */
        if (tmpdir_path[tmpdir_path_length-1] == '/') {

            /* "1" for '/', another "1" for '\0' */
            CALLOC_WITH_CHECK(hsperfdata_dir_path, tmpdir_path_length +
                    strlen(entry->d_name) + 1 + 1);
            sprintf(hsperfdata_dir_path, "%s%s/", 
                    tmpdir_path, entry->d_name);  

        } else {

            /* 2x"1" for '/', another "1" for '\0' */
            CALLOC_WITH_CHECK(hsperfdata_dir_path, 
                    tmpdir_path_length + 1 + strlen(entry->d_name) + 1 + 1);
            /* add an extra slash */
            sprintf(hsperfdata_dir_path, "%s/%s/", 
                    tmpdir_path, entry->d_name);

        }

        /* open hsperfdata dir */
        struct dirent ** subentrys;
        int n = scandir(hsperfdata_dir_path, &subentrys, __selector_2, alphasort);
        while (n --> 0) {
            struct dirent * subentry = subentrys[n];

            cjstat_jvm_pid * t = NULL;
            CALLOC_WITH_CHECK(t, sizeof(cjstat_jvm_pid));

            t->pid = atoi(subentry->d_name);
            
            unsigned long hsperfdata_dir_path_length = strlen(hsperfdata_dir_path);
            /* another "1" for '\0' */
            CALLOC_WITH_CHECK(t->path,
                    hsperfdata_dir_path_length + 1 + strlen(subentry->d_name));
            sprintf(t->path, "%s%s", hsperfdata_dir_path, subentry->d_name);  /* concate path */
            
            /* insert to link list */
            t->next = ret;
            ret = t;

            SAFE_FREE_CLEAN(subentry);
        }
        SAFE_FREE_CLEAN(subentrys);

        SAFE_FREE_CLEAN(entry);
        SAFE_FREE_CLEAN(hsperfdata_dir_path);
    }
    SAFE_FREE_CLEAN(entrys);

    return ret;
}



/* 
 * Returns a pid number which is on head node of a given linked list, and
 * removes the head node from the linked list.
 *
 * You can get the path by accessing the head node directly.
 *
 * Returns -1 when a null pointer is given.
 */
INLINE int get_next_jvm_pid(cjstat_jvm_pid ** restrict p) {
    if (NULL == *p) {
#ifndef NDEBUG
        fprintf(stderr, "get_next_pid() got a null pointer\n");
#endif  /* NDEBUG */
        return -1;
    }
    
    int ret = (*p)->pid;
    /* free path */
    if (NULL == (*p)->next) {  /* the last node */
        SAFE_FREE_CLEAN((*p)->path);
        SAFE_FREE_CLEAN(*p);  
    } else {  /* not the last node */
        /* It is pretty roundabout and complicated of the order of freeing
         * memory. Be careful. */
        cjstat_jvm_pid * to_delete=(*p)->next;
        SAFE_FREE_CLEAN((*p)->path);
        **p = *((*p)->next);
        SAFE_FREE_CLEAN(to_delete);
    }
    
    return ret;
}



/*
 * Returns path of the given pid.
 * 
 * Returns NULL if not existed.
 */
INLINE char * get_hsperfdata_path_by_pid(int pid) {
    cjstat_jvm_pid * pids=get_all_jvm_pid();

    while (NULL != pids) {
        if (pids->pid == pid) {
            /* copy */
            char * ret = NULL;
            CALLOC_WITH_CHECK(ret, strlen(pids->path) + 1);
            strcpy(ret, pids->path);

            /* free linked list */
            while (pids != NULL) {
                cjstat_jvm_pid * next = pids->next;
                SAFE_FREE_CLEAN(pids->path);
                SAFE_FREE_CLEAN(pids);
                pids = next;
            }

            return ret;
        }
        get_next_jvm_pid(&pids);
    }

    /* not found */ 

    /* free linked list */
    while (pids != NULL) {
        cjstat_jvm_pid * next = pids->next;
        SAFE_FREE_CLEAN(pids->path);
        SAFE_FREE_CLEAN(pids);
        pids = next;
    }
    return NULL;
}



/*
 * Returns a hsperfdata set as a hashmap.
 */
INLINE hsperfdata_hashmap * convert_hsperfdata_to_hashmap(hsperfdata * restrict hs) {
    hsperfdata_hashmap * ret = _hashmap_create();

    hsperfdata_entry * entry = hs->entry_list;

    while (NULL != entry) {
        _hashmap_put(ret, 
                _hashmap_entry_create((char *)(entry->key), &entry, 
                    sizeof(hsperfdata_entry *)));

        entry = entry->next;
    }

    return ret;
}

INLINE hsperfdata_entry *
get_hsperfdata_entry_from_hashmap(hsperfdata_hashmap * hm, const char * key) {
    _hashmap_entry * e1 = _hashmap_get(hm, key);
    hsperfdata_entry * e2 = *(hsperfdata_entry **)(e1->value);
    return e2;
}



INLINE bool 
__str_startswith(const char * restrict str, const char * restrict prefix) {
    do if ('\0' == *prefix)
        return true;
    while (*str++ == *prefix++);
    return false;
}



INLINE hsperfdata * get_hsperfdata_by_pid(int pid) {
    char * path = get_hsperfdata_path_by_pid(pid);

    hsperfdata * ret = get_hsperfdata_by_path(path);

    SAFE_FREE_CLEAN(path);

    return ret;
}



INLINE hsperfdata * get_hsperfdata_by_path(char * restrict hsperfdata_path) {

    /* get file length */
    struct stat st;
    stat(hsperfdata_path, &st);
    off_t file_length = st.st_size;

    /* read file */
    FILE * f = fopen(hsperfdata_path, "rb");
    assert(NULL != f && "failed to open file");
    byte d[file_length];
    fread(d, (size_t)file_length, 1, f);
    fclose(f);

    /* parse */
    hsperfdata * ret = create_hsperfdata();
    hsperfdata_parse(ret, d);
   
    return ret;
}



/* internal functions used for filtering directory */
INLINE int __selector(const struct dirent * restrict b) {
    return __str_startswith(b->d_name, "hsperfdata_");
}
INLINE int __selector_2(const struct dirent * restrict b) {
    if (b->d_name[0] == '.') {  
        /* skip for ".", ".." and hidden file */ 
        return 0;
    }
#ifndef NDEBUG
    /* validate filename */
    for (const char * p=b->d_name; *p!='\0'; ++p) {
        if (!isdigit(*p)) {
            fputs("the name of a hsperfdata file contains non digits", stderr);
            fprintf(stderr, "the name is %s\n", b->d_name);
            return 0;
        }
    }
#endif  /* NDEBUG */
    return 1;
}

#endif  /* HSPERFDATA_UTIL_H */

