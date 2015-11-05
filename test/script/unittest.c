#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "CuTest.h"

#include "cjstat/util.h"

#include "cjstat/_hashmap.h"

#define CUAssert(condition) CuAssert(tc, #condition, condition)

void Test__StrStartswith(CuTest * tc) {
    puts("Test__StrStartswith");
    
    CUAssert(false == __str_startswith("", "prefix"));
    CUAssert(true == __str_startswith("str", ""));

    CUAssert(true == __str_startswith("", ""));

    CUAssert(true == __str_startswith("cjstat_root", "cjstat_"));
    CUAssert(true == __str_startswith("cjstat_admin", "cjstat_"));
    CUAssert(true == __str_startswith("cjstat_zookeeper", "cjstat_"));
    CUAssert(false == __str_startswith("arubarubarubaruba", "cjstat_"));
    CUAssert(false == __str_startswith("cjstat000000", "cjstat_"));

    CUAssert(true == __str_startswith("cjstat_", "cjstat_"));

    CUAssert(false == __str_startswith("aruba", "cjstat_"));
    return;
}

void Test_hashmap_basic(CuTest * tc) {
    puts("Test_hashmap_basic");

    _hashmap_entry * e=NULL;
    const char * test_key="keykeykeykeykey";
    const char * test_value="shabishabishabi";

    /* test for _hashmap_entry_create() */
    e = _hashmap_entry_create(test_key, 
            (void *)test_value, strlen(test_value)+1);
    CUAssert(NULL != e);
    CUAssert(0 == strcmp(e->key, test_key));
    CUAssert(0 == strcmp((char *)e->value, test_value));
    CUAssert(strlen(test_value)+1 == e->length);

    /* test for _hashmap_entry_destroy() */
    _hashmap_entry_destroy(e);
    e = NULL;



    _hashmap * m=NULL;

    /* test for _hashmap_create() */
    m = _hashmap_create();
    CUAssert(NULL != m);

    /* test for _hashmap_init() */
    for (size_t i=0; i<HASHMAP_SIZE; ++i) {
        CUAssert(NULL == m->map[i]);
    }
    CUAssert(0 == m->size);

    /* test for _hashmap_destroy() */
    _hashmap_destroy(m);
    m = NULL;

    return;
}

void Test_hashmap_simple(CuTest * tc) {
    puts("Test_hashmap_simple");

    _hashmap * m = _hashmap_create();

    /* test for _hashmap_put() and _hashmap_get() */
    const int key_value_pair_num = 24;
    const char * key[] = {
        "ACE", "ADS", 
        "BC", 
        "CDN", "CMS", "CSS",
        "DPC", "DRDS", 
        "ECS", "ESS", 
        "KVStore", 
        "MNS", "MTS",
        "OAS", "OCS", "ODPS", "ONS", "OSS", "OTS", 
        "PTS",
        "RDS",  
        "SLB", "SLS", 
        "VPC"};
    const char * value[] = {
        "Aliyun Cloud Engine",
        "Analytic Database Service",
        "Batch Compute",
        "Content Delivery Network",
        "Cloud Monitor System",
        "Cloud Security Service",
        "Data Process Center",
        "Distribute Relational Database Service",
        "Elastic Compute Service",
        "Elastic Scaling Service",
        "Key-Value Store",
        "Message and Notification Service",
        "Multimedia Transcoding Service",
        "Open Archive Service",
        "Open Cache Service",
        "Open Data Processing Service",
        "Open Notification Service",
        "Object Storage Service",
        "Open Table Service",
        "Performance Test Service",
        "Relational Database Service",
        "Server Load Balancer",
        "Simple Log Service",
        "Virtual Private Cloud" };

    for (int i=0; i<key_value_pair_num; ++i) {
        _hashmap_put(
                m, 
                _hashmap_entry_create(
                    key[i], 
                    (void *)value[i],
                    strlen(value[i])+1));
    }

    /*
     *for (int i=0; i<HASHMAP_SIZE; ++i) {
     *    if (NULL != m->map[i]) {
     *        printf("%d : %s\n", i, m->map[i]->key);
     *    }
     *}
     */

    for (int i=0; i<key_value_pair_num; ++i) {
        _hashmap_entry * ret = _hashmap_get(m, key[i]);
        CUAssert(NULL != ret);
        /*
         *fprintf(stderr, "value[i] = %s\n", value[i]);
         *fprintf(stderr, "(char *)(ret->value) = %s\n", (char *)(ret->value));
         */
        CUAssert(0 == strcmp(value[i], (char *)(ret->value)));
    }

    _hashmap_destroy(m);

    return;
}

int main() {
    
    srand((unsigned int)time(NULL));

    CuString * output=CuStringNew();
    CuSuite * suite=CuSuiteNew();

    SUITE_ADD_TEST(suite, Test__StrStartswith);
    SUITE_ADD_TEST(suite, Test_hashmap_basic);
    SUITE_ADD_TEST(suite, Test_hashmap_simple);

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);

    return NULL != strstr(output->buffer, "F");
}

