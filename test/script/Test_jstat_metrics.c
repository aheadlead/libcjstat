#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <signal.h>

#include "cjstat/hsperfdata.h"
#include "cjstat/util.h"
#include "cjstat/jstat_metrics.h"

int main(int argc, char ** argv) {
    if (argc != 3) {
        fputs("param error", stderr);
        return 1;
    }

    char * jps_path = argv[1];
    pid_t pid = atoi(argv[2]);

    kill(pid, SIGSTOP);

    hsperfdata * hs = get_hsperfdata_by_pid((int)pid);
    hsperfdata_hashmap * hm = convert_hsperfdata_to_hashmap(hs);
    jstat_metrics metrics = get_jstat_metrics(hm);

    char cmd_buffer[1024] = "";
    char jstat_output_buffer[1024] = "";
    FILE * p;
    /*sleep(5);*/

#define GET_JSTAT(type)                                         \
    do {                                                        \
        sprintf(cmd_buffer, "%s " #type " %d", jps_path, pid);  \
        p = popen(cmd_buffer, "r");                             \
        /* skip the header */                                   \
        fgets(jstat_output_buffer, 1024, p);                    \
        fgets(jstat_output_buffer, 1024, p);                    \
        pclose(p);                                              \
    } while (0)

#define PRECISION 1e-1
#define ABS(x) (x>0?x:-x)
#define CHECK_DOUBLE(mtc)                                       \
    do {                                                        \
        if (ABS(jstat.mtc - metrics.mtc) > PRECISION) {         \
            puts("Wrong metric " #mtc ":");                     \
            printf("From jstat: %.2lf\tFrom cjstat: %.2lf\n",   \
                    jstat.mtc,                                  \
                    metrics.mtc);                               \
            putchar('\n');                                      \
            return 1;                                           \
        } else {                                                \
            puts("Pass: " #mtc);                                \
        }                                                       \
    } while (0)

#define CHECK_LONG(mtc)                                         \
    do {                                                        \
        if (jstat.mtc != metrics.mtc) {                         \
            puts("Wrong metric " #mtc ":");                     \
            printf("From jstat: %lld\tFrom cjstat: %lld\n",   \
                    jstat.mtc,                                  \
                    metrics.mtc);                               \
            putchar('\n');                                      \
            return 1;                                           \
        } else {                                                \
            puts("Pass: " #mtc);                                \
        }                                                       \
    } while (0)

#define MANUAL_CHECK_STRING(mtc)                                \
    do {                                                        \
        puts("Manual check: "#mtc);                             \
        puts(jstat_output_buffer);                              \
        printf(#mtc": %s\n", metrics.mtc);                      \
        printf("Pass? (press enter to pass)\a");                \
        if (getchar() != '\n')                                  \
            return 1;                                           \
        putchar('\n');                                          \
    } while (0)

    jstat_metrics jstat;
    
    GET_JSTAT       (-timestamp);
    sscanf(jstat_output_buffer, "%lf",
            &jstat.Timestamp);
    CHECK_DOUBLE    (Timestamp);

    GET_JSTAT       (-class);
    sscanf(jstat_output_buffer, "%lld %lf %lld %lf %lf",
            &jstat.Loaded,
            &jstat.Bytes,
            &jstat.Unloaded,
            &jstat.Bytes_2,
            &jstat.Time);
    CHECK_LONG      (Loaded);
    CHECK_DOUBLE    (Bytes);
    CHECK_LONG      (Unloaded);
    CHECK_DOUBLE    (Bytes_2);
    CHECK_DOUBLE    (Time);


    GET_JSTAT       (-compiler);
    sscanf(jstat_output_buffer, "%lld %lld %lld %*f %lld",
            &jstat.Compiled,
            &jstat.Failed,
            &jstat.Invalid,
            /* Time */
            &jstat.FailedType);
    CHECK_LONG      (Compiled);
    CHECK_LONG      (Failed);
    CHECK_LONG      (Invalid);
    MANUAL_CHECK_STRING(FailedMethod); 
    
    GET_JSTAT(-gc);
    sscanf(jstat_output_buffer, 
            "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lld %lf %lld %lf %lf",
            &jstat.S0C, &jstat.S1C,
            &jstat.S0U, &jstat.S1U,
            &jstat.EC, &jstat.EU,
            &jstat.OC, &jstat.OU,
            &jstat.PC, &jstat.PU,
            &jstat.YGC, &jstat.YGCT,
            &jstat.FGC, &jstat.FGCT,
            &jstat.GCT);
    CHECK_DOUBLE    (S0C);
    CHECK_DOUBLE    (S1C);
    CHECK_DOUBLE    (S0U);
    CHECK_DOUBLE    (S1U);
    CHECK_DOUBLE    (EC);
    CHECK_DOUBLE    (EU);
    CHECK_DOUBLE    (OC);
    CHECK_DOUBLE    (OU);
    CHECK_DOUBLE    (PC);
    CHECK_DOUBLE    (PU);
    CHECK_LONG      (YGC);
    CHECK_DOUBLE    (YGCT);
    CHECK_LONG      (FGC);
    CHECK_DOUBLE    (FGCT);
    CHECK_DOUBLE    (GCT);

    GET_JSTAT(-gccapacity);
    sscanf(jstat_output_buffer, 
            "%lf %lf %lf %*f %*f %*f %lf %lf %lf %*f "
            "%lf %lf %lf %*f %*d %*d",
            &jstat.NGCMN, &jstat.NGCMX, &jstat.NGC,
            /* S0C S1C EC */
            &jstat.OGCMN, &jstat.OGCMX, &jstat.OGC,
            /* OC */
            &jstat.PGCMN, &jstat.PGCMX, &jstat.PGC);
    CHECK_DOUBLE    (NGCMN);
    CHECK_DOUBLE    (NGCMX);
    CHECK_DOUBLE    (NGC);
    CHECK_DOUBLE    (OGCMN);
    CHECK_DOUBLE    (OGCMX);
    CHECK_DOUBLE    (OGC);
    CHECK_DOUBLE    (PGCMN);
    CHECK_DOUBLE    (PGCMX);
    CHECK_DOUBLE    (PGC);

    GET_JSTAT(-gccause);
    sscanf(jstat_output_buffer, 
            "%lf %lf %lf %lf %lf",
            &jstat.S0,
            &jstat.S1,
            &jstat.E,
            &jstat.O,
            &jstat.P
            /* YGC YGCT FGC FGCT GCT */);
    CHECK_DOUBLE    (S0);
    CHECK_DOUBLE    (S1);
    CHECK_DOUBLE    (E);
    CHECK_DOUBLE    (O);
    CHECK_DOUBLE    (P);
    MANUAL_CHECK_STRING(LGCC);
    MANUAL_CHECK_STRING(GCC);
            
    GET_JSTAT(-gcnew);
    sscanf(jstat_output_buffer, 
            "%*f %*f %*f %*f %lld %lld %lf",
            /* S0C S1C S0U S1U */
            &jstat.TT,
            &jstat.MTT,
            &jstat.DSS
            /* EC EU YGC YGT */);
    CHECK_LONG      (TT);
    CHECK_LONG      (MTT);
    CHECK_DOUBLE    (DSS);

    GET_JSTAT(-gcnewcapacity);
    sscanf(jstat_output_buffer, 
            "%*f %*f %*f %lf %*f %lf %*f %lf",
            /* NGCMN MGCMX NGC */
            &jstat.S0CMX,
            /* S0C */
            &jstat.S1CMX,
            /* S1C */
            &jstat.ECMX
            /* EC YGC FGC */);
    CHECK_DOUBLE    (S0CMX);
    CHECK_DOUBLE    (S1CMX);
    CHECK_DOUBLE    (ECMX);

    // gcold gcoldcapacity gcpermcapacity gcutil
    
    GET_JSTAT(-printcompilation);
    sscanf(jstat_output_buffer, 
            "%*d %lld %lld",
            &jstat.Size,
            &jstat.Type);
    CHECK_LONG      (Size);
    CHECK_LONG      (Type);
    MANUAL_CHECK_STRING(Method);

    destroy_hsperfdata(hs);

    kill(pid, SIGCONT);

    puts("Test jstat_metrcis is passed.");

    return 0;
}

