# libcjstat

A C library to get jstat-like metrics for running jvms.

The _jstat_ tool displays performance statistics for an instrumented HotSpot Java
virtual machine (JVM). Java developers concern performance statistics when
performance tuning or tracking memory leak problem. Tool _jstat_ from JDK is
easy-to-use and useful when coping with a single JVM. But _jstat_ is a java
program itself, cost highly when raising a _jstat_, it is hard to track lots
of JVMs in the cluster. 

With _libcjstat_, you can build a JVM cluster monitor easily, collecting the
statistics without high cost, storing them in [InfluxDB](https://influxdb.com),
visualizing them by [Grafana](http://grafana.org).

Version: 0.1

## Features

* Get all jstat metrics via a simple function call.
* Get started with including some headers simply.
* No external dependencies.
* Tested.

## Requirements

* GNU99 C Compiler
* Java 7
* Linux or OS X (No support for Windows)

## How to start

1.  Copy folder cjstat/ to your project.
2.  Include headers in your code.
3.  Done.

## Typical usages

Suppose a java program is running (pid=14548) for demonstrations.

### Get jstat metrics

```c
#include <stdio.h>
#include "cjstat/hsperfdata.h"
#include "cjstat/jstat_metrics.h"

int main() {

    hsperfdata * hs = get_hsperfdata_by_pid(14548);
    hsperfdata_hashmap * hm = convert_hsperfdata_to_hashmap(hs);
    jstat_metrics jm = get_jstat_metrics(hm);

    printf("S0C = %.4lf\n", jm.S0C);
    printf("S1C = %.4lf\n", jm.S1C);
    printf("S0U = %.4lf\n", jm.S0U);
    printf("S1U = %.4lf\n", jm.S1U);
    printf("EC = %.4lf\n", jm.EC);
    printf("EU = %.4lf\n", jm.EU);
    printf("OC = %.4lf\n", jm.OC);
    printf("OU = %.4lf\n", jm.OU);
    printf("PC = %.4lf\n", jm.PC);
    printf("PU = %.4lf\n", jm.PU);
    printf("YGC = %lld\n", jm.YGC);
    printf("YGCT = %.4lf\n", jm.YGCT);
    printf("FGC = %lld\n", jm.FGC);
    printf("FGCT = %.4lf\n", jm.FGCT);
    printf("GCT = %.4lf\n", jm.GCT);

    _hashmap_destroy(hm);
    destroy_hsperfdata(hs);

    return 0;
}

```

Output

```
S0C = 6528.0000
S1C = 6528.0000
S0U = 0.0000
S1U = 0.0000
EC = 52480.0000
EU = 3148.9531
OC = 5312.0000
OU = 0.0000
PC = 21248.0000
PU = 4954.5078
YGC = 0
YGCT = 0.0000
FGC = 0
FGCT = 0.0000
GCT = 0.0000
``` 

### Get pids of all running JVMs

```c
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
```
Output

```
14548 /var/folders/xx/pxh4yd_56yn8lsx944bvv_s80000gn/T/hsperfdata_weiyulan/14548
```

# To-do

* Write docs
* Make it compatible with standard C99 compiler (small changes)
* Make it compatible with Java 8
* Write a essay to explain how hsperfdata works

# Contributor

aheadlead
email: aheadlead@dlifep.com

A junior student in NUAA, China.

# License

MIT

# What's more?

This project originates from my intern work, with my rewriting in C for a
month, at the end of my sophomore year.

I know the importance of codes quailty, and did write codes with heart and soul.
Because it is the FIRST open source project since I wrote codes. It is inevitable
that making mistakes when coding. If you have any problem or question, please
send mails or issue to me so that let me know. 

Thank you very much.

