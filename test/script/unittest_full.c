#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <signal.h>

#include "CuTest.h"

#include "cjstat/util.h"
#include "cjstat/hsperfdata.h"
#include "cjstat/jstat_metrics.h"
#include "cjstat/_hashmap.h"

#include "mytest.h"

#define CUAssert(condition) CuAssert(tc, #condition, condition)

void Test_hashmap_large(CuTest * tc) {
    tc = tc + 1 - 1;  // suppress unused variable compiler warning

    puts("Test_hashmap_large");

    /* quantity test */
#define CYCLE 300
    int cycle = CYCLE;
    const int pair_num = HASHMAP_SIZE;
    while (cycle --> 0) {
        if ((int)(10.0*cycle/CYCLE) != (int)(10.0*(cycle+1)/CYCLE)) {
            printf("%d ", cycle+1);
            fflush(stdout);
        }
        _hashmap * m = _hashmap_create();
        int rand_values[HASHMAP_SIZE] = {0};

        for (int i=0; i<pair_num; ++i) {
            char buffer[11];  /* max: 2147483647 */
            int rand_value = rand();
            rand_values[i] = rand_value;
            sprintf(buffer, "%d", rand_value);
            _hashmap_put(
                    m,
                    _hashmap_entry_create(
                        buffer,
                        (void *)&rand_value,
                        sizeof(int)));
            CUAssert(*(int *)(_hashmap_get(m, buffer)->value) == rand_values[i]);
        }
        _hashmap_destroy(m);
    }

    putchar('\n');
#undef CYCLE

    return;
}

int main() {
    signal(SIGQUIT, signal_handler);
    srand((unsigned int)time(NULL));

    CuString * output=CuStringNew();
    CuSuite * suite=CuSuiteNew();

    SUITE_ADD_TEST(suite, Test_hashmap_large);

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

    return NULL != strstr(output->buffer, "F");
}

