#ifndef PERFTEST_H
#define PERFTEST_H

#include <time.h>
#include <string.h>
#include <stdlib.h>

static char __testname[1024];
static char __commit_id[41];
static int __testcycle = 1000;
static clock_t __start;
static clock_t __duration_clocks;

#define PREPARE_TEST(x, tc)                                     \
    {                                                           \
        strcpy(__testname, (x));                                \
        __testcycle = (tc);                                     \
        printf("Test \"%s\"\n", __testname);                    \
        FILE * f = popen("git rev-parse HEAD", "r");            \
        fgets(__commit_id, 40, f);                              \
        pclose(f);                                              \
        printf("Cycle:\t\t%-8d\tCommit ID:\t%s\n",              \
                __testcycle, __commit_id);                      \
        fflush(stdout);                                         \
    }
#define START_TEST                                              \
    {                                                           \
        __duration_clocks = 0;                                  \
        __start = clock();                                      \
    }                                                           \
    for (int __tc=0; __tc<__testcycle; ++__tc) {
#define PAUSE_TEST                                              \
    {                                                           \
        __duration_clocks += clock() - __start;                 \
    }
#define RESUME_TEST                                             \
    {                                                           \
        __start = clock();                                      \
    }
#define STOP_TEST                                               \
    }                                                           \
    {                                                           \
        __duration_clocks += clock() - __start;                 \
    }
#define SHOW_TEST()                                             \
    do {                                                        \
        double __duration = 0.0;                                \
        __duration = (double)__duration_clocks/CLOCKS_PER_SEC;  \
        char __buffer[1024];                                    \
        snprintf(__buffer, 1024,                                \
                "Duration:\t%-.5f s\tSingle:\t\t%-.4f ms\n"     \
                "Score:\t\t%-.5f operations per second\n"       \
                "\n",                                           \
                __duration,                                     \
                1000.0 * __duration / __testcycle,              \
                1.0 / (__duration / __testcycle));              \
        char __path_buffer[1024];                               \
        snprintf(__path_buffer, 1024,                           \
                "log/perftest.%s.log",                          \
                __testname);                                    \
        FILE * __logout = fopen(__path_buffer, "a");            \
        fputs(__buffer, stdout);                                \
        fputs(__buffer, __logout);                              \
        fclose(__logout);                                       \
    } while (0)

#define RANDINIT()          srand((unsigned int)time(NULL))
#define RANDINT(a, b)       (rand()%(b-a)+a)
#define RANDSTRING(str, len)                                    \
    do {                                                        \
        str[len] = '\0';                                        \
        for (int _RANDSTRING_i=0; _RANDSTRING_i<len;            \
                ++_RANDSTRING_i)                                \
            str[_RANDSTRING_i] = (char)RANDINT('a', 'z');       \
    } while (0)
            

#endif  /* PERFTEST_H */

