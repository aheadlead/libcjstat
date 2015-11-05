#ifndef JSTAT_METRICS_H
#define JSTAT_METRICS_H

#include "util.h"
#include "_alloc.h"
#include "_hashmap.h"
#include "hsperfdata.h"

typedef struct {

/* timestamp */

    /* Timestamp (sec) = sun.os.hrt.ticks/sun.os.hrt.frequency
     */
    double Timestamp;

/* class */

    /* Loaded = java.cls.loadedClasses + java.cls.sharedLoadedClasses
     * Number of classes loaded
     */
    unsigned long long Loaded;

    /* Bytes (K) = sun.cls.loadedBytes + sun.cls.sharedLoadedBytes
     * Accumulated Size of classes loaded
     */
    double Bytes;

    /* Unloaded = java.cls.unloadedClasses + java.cls.sharedUnloadedClasses
     * Number of classes unloaded 
     */
    unsigned long long Unloaded;
    
    /* Bytes_2 (K) = sun.cls.unloadedBytes + sun.cls.sharedUnloadedBytes
     * Accumulated size of classes unloaded
     */
    double Bytes_2;
    
    /* Time = sun.cls.time/sun.os.hrt.frequency
     * Accumulated time for class loading
     */
    double Time;

/* compiler */

    /* Compiled = sun.ci.totalCompiles
     * Number of compilations task performed
     */
    unsigned long long Compiled;

    /* Failed = sun.ci.totalBailouts
     * Number of failed compilation tasks
     */
    unsigned long long Failed;

    /* Invalid = sun.ci.totalInvalidates
     * Number of invalidated compilation tasks
     */
    unsigned long long Invalid;

    /* Time */

    /* FailedType = sun.ci.lastFailedType
     * Type of last failed compilation
     */
    unsigned long long FailedType;
    
    /* FailedMethod = sun.ci.lastFailedMethod
     * Name of class and method for last failed compile */
    char * FailedMethod;

/* gc */

    /* S0C (K) = sun.gc.generation.0.space.1.capacity
     * Survivor 0 Space Capacity - Current
     */
    double S0C;

    /* S1C (K) = sun.gc.generation.0.space.2.capacity
     * Survivor 1 Space Capacity - Current
     */
    double S1C;

    /* S0U (K) = sun.gc.generation.0.space.1.used
     * Survivor 0 Space Used
     */
    double S0U;
    
    /* S1U (K) = sun.gc.generation.0.space.2.used
     * Survivor 1 Space Used
     */
    double S1U;

    /* EC (K) = sun.gc.generation.0.space.0.capacity
     * Eden Space Capacity - Current
     */
    double EC;
    
    /* EU (K) = sun.gc.generation.0.space.0.used
     * Eden Space Used
     */
    double EU;

    /* OC (K) = sun.gc.generation.1.space.0.capacity
     * Old Space Capacity - Current
     */
    double OC;
    
    /* OU (K) = sun.gc.generation.1.space.0.used
     * Old Space Used
     */
    double OU;

    /* PC (K) = sun.gc.generation.2.space.0.capacity
     * Perm Space Capacity - Current
     *
     */
    double PC;
    
    /* PU (K) = sun.gc.generation.2.space.0.used
     * Perm Space Used
     */
    double PU;

    /* YGC = sun.gc.collector.0.invocations
     * Young Generation Collections
     */
    unsigned long long YGC;

    /* YGCT = sun.gc.collector.0.time/sun.os.hrt.frequency
     * Young Generation Garbage Collection Time
     */
    double YGCT;

    /* FGC = sun.gc.collector.1.invocations
     * Full Generation Collections
     */
    unsigned long long FGC;

    /* FGCT = sun.gc.collector.1.time/sun.os.hrt.frequency
     * Full Generation Garbage Collection Time
     */
    double FGCT;

    /* GCT = (sun.gc.collector.0.time + sun.gc.collector.1.time) /
     *       sun.os.hrt.frequency
     * Total Garbage Collection Time
     */
    double GCT;

/* gccapacity */

    /* NGCMN (K) = sun.gc.generation.0.minCapacity
     * New Generation Capacity - Minimum
     */
    double NGCMN;

    /* NGCMX (K) = sun.gc.generation.0.maxCapacity
     * New Generation Capacity - Maximum
     */
    double NGCMX;

    /* NGC (K) = sun.gc.generation.0.capacity
     * New Generation Capacity - Current
     */
    double NGC;
    
    /* S0C */
    /* S1C */
    /* EC */

    /* OGCMN (K) = sun.gc.generation.1.minCapacity
     * Old Generation Capacity - Minimum
     */
    double OGCMN;

    /* OGCMX (K) = sun.gc.generation.1.maxCapacity
     * Old Generation Capacity - Maximum
     */
    double OGCMX;

    /* OGC (K) = sun.gc.generation.1.capacity
     * Old Generation Capacity - Current
     */
    double OGC;

    /* OC */

    /* PGCMN (K) = sun.gc.generation.2.minCapacity
     * Perm Generation Capacity - Minimum
     */
    double PGCMN;

    /* PGCMX (K) = sun.gc.generation.2.maxCapacity
     * Perm Generation Capacity - Maximum
     */
    double PGCMX;

    /* PGC (K) = sun.gc.generation.2.capacity
     * Perm Generation Capacity - Current
     */
    double PGC;

    /* PC */
    /* YGC */
    /* FGC */

/* gccause */

    /* S0 = (1 - ((sun.gc.generation.0.space.1.capacity -
     *      sun.gc.generation.0.space.1.used) /
     *      sun.gc.generation.0.space.1.capacity)) * 100
     * Survivor 0 Space - Percent Used
     */
    double S0;

    /* S1 = (1 - ((sun.gc.generation.0.space.2.capacity -
     *      sun.gc.generation.0.space.2.used) /
     *      sun.gc.generation.0.space.2.capacity)) * 100
     * Survivor 1 Space - Percent Used
     */
    double S1;

    /* E = (1 - ((sun.gc.generation.0.space.0.capacity -
     *      sun.gc.generation.0.space.0.used) /
     *      sun.gc.generation.0.space.0.capacity)) * 100
     * Eden Space - Percent Used
     */
    double E;

    /* O = (1 - ((sun.gc.generation.1.space.0.capacity -
     *      sun.gc.generation.1.space.0.used) /
     *      sun.gc.generation.1.space.0.capacity)) * 100
     * Old Space - Percent Used
     */
    double O;

    /* P = (1 - ((sun.gc.generation.2.space.0.capacity -
     *      sun.gc.generation.2.space.0.used) /
     *      sun.gc.generation.2.space.0.capacity)) * 100
     * Perm Space - Percent Used
     */
    double P;

    /* YGC */
    /* YGCT */
    /* FGC */
    /* FGCT */
    /* GCT */

    /* LGCC = sun.gc.lastCause
     * Last GC Cause
     */
    char * LGCC;

    /* GCC = sun.gc.cause
     * Current GC Cause
     */
    char * GCC;

/* gcnew */

    /* S0C */
    /* S1C */
    /* S0U */
    /* S1U */

    /* TT = sun.gc.policy.tenuringThreshold
     * Tenuring Threshold
     */
    unsigned long long TT;

    /* MTT = sun.gc.policy.maxTenuringThreshold
     * Maximum Tenuring Threshold
     */
    unsigned long long MTT;

    /* DSS (K) = sun.gc.policy.desiredSurvivorSize
     * Desired Survivor Size
     */
    double DSS;

    /* EC */
    /* EU */
    /* YGC */
    /* YGCT */

/* gcnewcapacity */

    /* NGCMN */
    /* NGCMX */
    /* NGC */

    /* S0CMX (K) = sun.gc.generation.0.space.1.maxCapacity
     * Survivor 0 Space Capacity - Maximum
     */
    double S0CMX;

    /* S0C */
 
    /* S1CMX (K) = sun.gc.generation.0.space.2.maxCapacity
     * Survivor 1 Space Capacity - Maximum
     */
    double S1CMX;

    /* S1C */

    /* ECMX (K) = sun.gc.generation.0.space.0.maxCapacity
     * Eden Space Capacity - Maximum
     */
    double ECMX;

    /* EC */
    /* YGC */
    /* FGC */

/* gcold */

    /* PC */
    /* PU */
    /* OC */
    /* OU */
    /* YGC */
    /* FGC */
    /* FGCT */
    /* GCT */

/* gcoldcapacity */

    /* OGCMN */
    /* OGCMX */
    /* OGC */
    /* OC */
    /* YGC */
    /* FGC */
    /* FGCT */
    /* GCT */

/* gcpermcapacity */

    /* PGCMN */
    /* PGCMX */
    /* PGC */
    /* PC */
    /* YGC */
    /* FGC */
    /* FGCT */
    /* GCT */

/* gcutil */

    /* S0 */
    /* S1 */
    /* E */
    /* O */
    /* P */
    /* YGC */
    /* YGCT */
    /* FGC */
    /* FGCT */
    /* GCT */

/* printcompilation */
    
    /* Compiled */

    /* Size = sun.ci.lastSize
     * Code Size in bytes of last compilation
     */
    unsigned long long Size;

    /* Type = sun.ci.lastMethod
     * Type of last compilation
     */
    unsigned long long Type;

    /* Method = sun.ci.lastMethod
     * Name of class and method for last compile
     */
    char * Method;

} jstat_metrics;

jstat_metrics get_jstat_metrics(hsperfdata_hashmap * m);

inline jstat_metrics
get_jstat_metrics(hsperfdata_hashmap * m) {

    jstat_metrics r;
    
    // acquire fields
    qword java_cls_loadedClasses = 
        get_hsperfdata_entry_from_hashmap(m, "java.cls.loadedClasses")->value_long;

    qword java_cls_sharedLoadedClasses = 
        get_hsperfdata_entry_from_hashmap(m, "java.cls.sharedLoadedClasses")->value_long;

    qword java_cls_sharedUnloadedClasses = 
        get_hsperfdata_entry_from_hashmap(m, "java.cls.sharedUnloadedClasses")->value_long;

    qword java_cls_unloadedClasses = 
        get_hsperfdata_entry_from_hashmap(m, "java.cls.unloadedClasses")->value_long;

    byte * sun_ci_lastFailedMethod = 
        get_hsperfdata_entry_from_hashmap(m, "sun.ci.lastFailedMethod")->value_bytearray;

    qword sun_ci_lastFailedType = 
        get_hsperfdata_entry_from_hashmap(m, "sun.ci.lastFailedType")->value_long;

    byte * sun_ci_lastMethod = 
        get_hsperfdata_entry_from_hashmap(m, "sun.ci.lastMethod")->value_bytearray;

    qword sun_ci_lastSize = 
        get_hsperfdata_entry_from_hashmap(m, "sun.ci.lastSize")->value_long;

    qword sun_ci_lastType = 
        get_hsperfdata_entry_from_hashmap(m, "sun.ci.lastType")->value_long;

    qword sun_ci_totalBailouts = 
        get_hsperfdata_entry_from_hashmap(m, "sun.ci.totalBailouts")->value_long;

    qword sun_ci_totalCompiles = 
        get_hsperfdata_entry_from_hashmap(m, "sun.ci.totalCompiles")->value_long;

    qword sun_ci_totalInvalidates = 
        get_hsperfdata_entry_from_hashmap(m, "sun.ci.totalInvalidates")->value_long;

    qword sun_cls_loadedBytes = 
        get_hsperfdata_entry_from_hashmap(m, "sun.cls.loadedBytes")->value_long;

    qword sun_cls_sharedLoadedBytes = 
        get_hsperfdata_entry_from_hashmap(m, "sun.cls.sharedLoadedBytes")->value_long;

    qword sun_cls_sharedUnloadedBytes = 
        get_hsperfdata_entry_from_hashmap(m, "sun.cls.sharedUnloadedBytes")->value_long;

    qword sun_cls_time = 
        get_hsperfdata_entry_from_hashmap(m, "sun.cls.time")->value_long;

    qword sun_cls_unloadedBytes = 
        get_hsperfdata_entry_from_hashmap(m, "sun.cls.unloadedBytes")->value_long;

    byte * sun_gc_cause = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.cause")->value_bytearray;

    qword sun_gc_collector_0_invocations = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.collector.0.invocations")->value_long;

    qword sun_gc_collector_0_time = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.collector.0.time")->value_long;

    qword sun_gc_collector_1_invocations = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.collector.1.invocations")->value_long;

    qword sun_gc_collector_1_time = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.collector.1.time")->value_long;

    qword sun_gc_generation_0_capacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.capacity")->value_long;

    qword sun_gc_generation_0_maxCapacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.maxCapacity")->value_long;

    qword sun_gc_generation_0_minCapacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.minCapacity")->value_long;

    qword sun_gc_generation_0_space_0_capacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.space.0.capacity")->value_long;

    qword sun_gc_generation_0_space_0_maxCapacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.space.0.maxCapacity")->value_long;

    qword sun_gc_generation_0_space_0_used = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.space.0.used")->value_long;

    qword sun_gc_generation_0_space_1_capacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.space.1.capacity")->value_long;

    qword sun_gc_generation_0_space_1_maxCapacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.space.1.maxCapacity")->value_long;

    qword sun_gc_generation_0_space_1_used = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.space.1.used")->value_long;

    qword sun_gc_generation_0_space_2_capacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.space.2.capacity")->value_long;

    qword sun_gc_generation_0_space_2_maxCapacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.space.2.maxCapacity")->value_long;

    qword sun_gc_generation_0_space_2_used = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.0.space.2.used")->value_long;

    qword sun_gc_generation_1_capacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.1.capacity")->value_long;

    qword sun_gc_generation_1_maxCapacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.1.maxCapacity")->value_long;

    qword sun_gc_generation_1_minCapacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.1.minCapacity")->value_long;

    qword sun_gc_generation_1_space_0_capacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.1.space.0.capacity")->value_long;

    qword sun_gc_generation_1_space_0_used = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.1.space.0.used")->value_long;

    qword sun_gc_generation_2_capacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.2.capacity")->value_long;

    qword sun_gc_generation_2_maxCapacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.2.maxCapacity")->value_long;

    qword sun_gc_generation_2_minCapacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.2.minCapacity")->value_long;

    qword sun_gc_generation_2_space_0_capacity = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.2.space.0.capacity")->value_long;

    qword sun_gc_generation_2_space_0_used = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.generation.2.space.0.used")->value_long;

    byte * sun_gc_lastCause = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.lastCause")->value_bytearray;

    qword sun_gc_policy_desiredSurvivorSize = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.policy.desiredSurvivorSize")->value_long;

    qword sun_gc_policy_maxTenuringThreshold = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.policy.maxTenuringThreshold")->value_long;

    qword sun_gc_policy_tenuringThreshold = 
        get_hsperfdata_entry_from_hashmap(m, "sun.gc.policy.tenuringThreshold")->value_long;

    qword sun_os_hrt_frequency = 
        get_hsperfdata_entry_from_hashmap(m, "sun.os.hrt.frequency")->value_long;

    qword sun_os_hrt_ticks = 
        get_hsperfdata_entry_from_hashmap(m, "sun.os.hrt.ticks")->value_long;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

    // Compute
    // timastamp
    r.Timestamp = (double)sun_os_hrt_ticks / sun_os_hrt_frequency;

    // class
    r.Loaded = (java_cls_loadedClasses + java_cls_sharedLoadedClasses);
    r.Bytes = (sun_cls_loadedBytes + sun_cls_sharedLoadedBytes) / 1024.0;
    r.Unloaded = (java_cls_unloadedClasses + java_cls_sharedUnloadedClasses);
    r.Bytes_2 = (sun_cls_unloadedBytes + sun_cls_sharedUnloadedBytes) / 1024.0;
    r.Time = (double)sun_cls_time / sun_os_hrt_frequency;

    // compiler
    r.Compiled = sun_ci_totalCompiles;
    r.Failed = sun_ci_totalBailouts;
    r.Invalid = sun_ci_totalInvalidates;
    /* r.Time */
    r.FailedType = sun_ci_lastFailedType;
    r.FailedMethod = (char *)sun_ci_lastFailedMethod;

    // gc
    r.S0C = sun_gc_generation_0_space_1_capacity / 1024.0;
    r.S1C = sun_gc_generation_0_space_2_capacity / 1024.0;
    r.S0U = sun_gc_generation_0_space_1_used / 1024.0;
    r.S1U = sun_gc_generation_0_space_2_used / 1024.0;
    r.EC = sun_gc_generation_0_space_0_capacity / 1024.0;
    r.EU = sun_gc_generation_0_space_0_used / 1024.0;
    r.OC = sun_gc_generation_1_space_0_capacity / 1024.0;
    r.OU = sun_gc_generation_1_space_0_used / 1024.0;
    r.PC = sun_gc_generation_2_space_0_capacity / 1024.0;
    r.PU = sun_gc_generation_2_space_0_used / 1024.0;
    r.YGC = sun_gc_collector_0_invocations;
    r.YGCT = (double)sun_gc_collector_0_time / sun_os_hrt_frequency;
    r.FGC = sun_gc_collector_1_invocations;
    r.FGCT = (double)sun_gc_collector_1_time / sun_os_hrt_frequency;
    r.GCT = (double)(sun_gc_collector_0_time + sun_gc_collector_1_time) /
        sun_os_hrt_frequency;

    // gccapacity
    
    r.NGCMN = (double)sun_gc_generation_0_minCapacity / 1024.0;
    r.NGCMX = (double)sun_gc_generation_0_maxCapacity / 1024.0;
    r.NGC = (double)sun_gc_generation_0_capacity / 1024.0;
    /* S0C */
    /* S1C */
    /* EC */
    r.OGCMN = (double)sun_gc_generation_1_minCapacity / 1024.0;
    r.OGCMX = (double)sun_gc_generation_1_maxCapacity / 1024.0;
    r.OGC = (double)sun_gc_generation_1_capacity / 1024.0;
    r.PGCMN = (double)sun_gc_generation_2_minCapacity / 1024.0;
    r.PGCMX = (double)sun_gc_generation_2_maxCapacity / 1024.0;
    r.PGC = (double)sun_gc_generation_2_capacity / 1024.0;
    /* PC */
    /* YGC */
    /* FGC */

    // gccause
    r.S0 = (1.0 - ((double)(sun_gc_generation_0_space_1_capacity -
                    sun_gc_generation_0_space_1_used) /
                sun_gc_generation_0_space_1_capacity)) * 100.0;
    r.S1 = (1.0 - ((double)(sun_gc_generation_0_space_2_capacity -
                    sun_gc_generation_0_space_2_used) /
                sun_gc_generation_0_space_2_capacity)) * 100.0;
    r.E = (1.0 - ((double)(sun_gc_generation_0_space_0_capacity -
                    sun_gc_generation_0_space_0_used) /
                sun_gc_generation_0_space_0_capacity)) * 100.0;
    r.O = (1.0 - ((double)(sun_gc_generation_1_space_0_capacity -
                    sun_gc_generation_1_space_0_used) /
                sun_gc_generation_1_space_0_capacity)) * 100.0;
    r.P = (1.0 - ((double)(sun_gc_generation_2_space_0_capacity -
                    sun_gc_generation_2_space_0_used) /
                sun_gc_generation_2_space_0_capacity)) * 100.0;
    /* YGC */
    /* YGCT */
    /* FGC */
    /* FGCT */
    /* GCT */
    r.LGCC = (char *)sun_gc_lastCause;
    r.GCC = (char *)sun_gc_cause;

    // gcnew
    /* S0C */
    /* S1C */
    /* S0U */
    /* S1U */
    r.TT = sun_gc_policy_tenuringThreshold;
    r.MTT = sun_gc_policy_maxTenuringThreshold;
    r.DSS = sun_gc_policy_desiredSurvivorSize / 1024.0;
    /* EC */
    /* EU */
    /* YGC */
    /* YGCT */

    // gcnewcapacity
    /* NGCMN */
    /* NGCMX */
    /* NGC */
    r.S0CMX = sun_gc_generation_0_space_1_maxCapacity / 1024.0;
    /* S0C */
    r.S1CMX = sun_gc_generation_0_space_2_maxCapacity / 1024.0;
    /* S1C */
    r.ECMX = sun_gc_generation_0_space_0_maxCapacity / 1024.0;
    /* EC */
    /* YGC */
    /* FGC */


    // gcold 
    /* PC */
    /* PU */
    /* OC */
    /* OU */
    /* YGC */
    /* FGC */
    /* FGCT */
    /* GCT */

    // gcoldcapacity 
    /* OGCMN */
    /* OGCMX */
    /* OGC */
    /* OC */
    /* YGC */
    /* FGC */
    /* FGCT */
    /* GCT */

    // gcpermcapacity 
    /* PGCMN */
    /* PGCMX */
    /* PGC */
    /* PC */
    /* YGC */
    /* FGC */
    /* FGCT */
    /* GCT */

    // gcutil 
    /* O */
    /* P */
    /* YGC */
    /* YGCT */
    /* FGC */
    /* FGCT */
    /* GCT */
    
    // printcompilation
    /* Compiled */
    r.Size = sun_ci_lastSize;
    r.Type = sun_ci_lastType;
    r.Method = (char *)sun_ci_lastMethod;

#pragma GCC diagnostic pop

    return r;
    
}  // the end of definition get_jstat_metrics()

#endif  /* JSTAT_METRICS_H */

