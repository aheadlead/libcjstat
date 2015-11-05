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
