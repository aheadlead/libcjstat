#include <stdio.h>

#include "cjstat/util.h"

int main() {
    cjstat_jvm_pid * pids = get_all_jvm_pid();

    while (NULL != pids) {
        printf("%d %s\n", pids->pid, pids->path);
        get_next_jvm_pid(&pids);
    }

    return 0;
}

