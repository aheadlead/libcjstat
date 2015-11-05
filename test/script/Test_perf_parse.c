#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "cjstat/hsperfdata.h"
#include "cjstat/parser.h"
#include "cjstat/util.h"
#include "cjstat/_alloc.h"

#include "perftest.h"

int main(int argc, char ** argv) {
    if (2 != argc) {
        fputs("param error\n", stderr);
        exit(1);
    }

    PREPARE_TEST("parse a single static perfdata file", 1000) {
        
        FILE * f = fopen("resource/hsperfdata_sample/2956", "rb");
        byte d[32768];

        fread(d, 32768, 1, f);
        fclose(f);

        START_TEST

        PAUSE_TEST
        hsperfdata * hs = create_hsperfdata();
        RESUME_TEST
        
        hsperfdata_parse(hs, d);
        
        PAUSE_TEST
        destroy_hsperfdata(hs);
        RESUME_TEST

        STOP_TEST

    } SHOW_TEST();

    PREPARE_TEST("parse a real jvm", 1000) {

        pid_t child_pid = fork();
        if (child_pid < 0) {  /* error */

            perror("fork");
            exit(1);

        } else if (child_pid == 0) {

            execl(argv[1], argv[1], "-server", "-cp", "tmp", "-Xmx256m",
                    "-Xmn64m", "-XX:MaxPermSize=64m",
                    "-XX:ParallelGCThreads=2", "-XX:+UseConcMarkSweepGC",
                    "-XX:+UseCMSInitiatingOccupancyOnly",
                    "-XX:CMSInitiatingOccupancyFraction=70",
                    "-XX:CMSFullGCsBeforeCompaction=5",
                    "-XX:+UseCMSCompactAtFullCollection",
                    "-XX:CMSMaxAbortablePrecleanTime=5000", "-verbose:gc",
                    "do_nothing", NULL);
        }

        sleep(1);  /* waiting for java */

        START_TEST
        hsperfdata * hs = get_hsperfdata_by_pid(child_pid);
        destroy_hsperfdata(hs);
        STOP_TEST

        kill(child_pid, SIGKILL);

    } SHOW_TEST();

    return 0;
}

