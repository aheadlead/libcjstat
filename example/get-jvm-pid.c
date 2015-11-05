#include <stdio.h>
#include "cjstat/util.h"

int main() {

    for (cjstat_jvm_pid * pids = get_all_jvm_pid(); 
            NULL != pids; 
            get_next_jvm_pid(&pids)) {
        printf("%d\t%s\n", pids->pid, pids->path);
    }

    return 0;
}

