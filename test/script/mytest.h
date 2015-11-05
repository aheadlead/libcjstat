#ifndef MYTEST_H
#define MYTEST_H

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void signal_handler(int signo) {
    if (SIGQUIT == signo) {
        exit(0);
    }
}

#endif  /* MYTEST_H */

