#ifndef MYTEST_H
#define MYTEST_H

#include <mylib/common.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdarg.h>
#include <assert.h>

MYLIB_BEGIN_C_DECLS

enum {
    TEST_STATUS_UNKNOWN = 0,
    TEST_STATUS_PASSED = 1,
    TEST_STATUS_FAILED = 2,
    TEST_STATUS_SKIPPED = 3,
};

struct assert_stats {
    const char* file;
    int line;
    bool passed;
    const char* message;
    struct assert_stats* next;
};

struct test_stats {
    const char* name;
    const char* suite;
    const char* id;
    const char* file;
    int line;

    void (* run)(void);

    void (* init)(void);

    void (* shutdown)(void);

    unsigned passed_asserts;
    unsigned failed_asserts;
    int test_status;
    const char* message;
    float elapsed_time;
    bool crashed;
    bool timed_out;
    struct assert_stats* asserts;
    struct test_stats* next;
};

struct suite_stats {
    const char* name;
    const char* file;
    int line;

    unsigned tests_total;
    unsigned tests_passed;
    unsigned tests_failed;
    unsigned tests_crashed;
    unsigned tests_skipped;
    struct test_stats* tests;
    struct suite_stats* next;
};

struct global_stats {
    unsigned tests_total;
    unsigned tests_passed;
    unsigned tests_failed;
    unsigned tests_crashed;
    unsigned tests_skipped;
    struct suite_stats* suites;
};

enum {
    POOL_ASSERTS_MAX = 4096,
    POOL_SUITES_MAX = 4096,
};

static struct assert_stats pool_asserts[POOL_ASSERTS_MAX];
static uint32_t pool_asserts_next = 0;

static struct suite_stats pool_suites[POOL_SUITES_MAX];
static uint32_t pool_suites_next = 0;

static struct test_stats* current_test_stats;
static struct suite_stats* current_suite_stats;
static struct global_stats global_stats;

inline static struct assert_stats* create_assert(void) {
    assert(pool_asserts_next < POOL_ASSERTS_MAX);
    struct assert_stats* as = &pool_asserts[pool_asserts_next++];
    struct assert_stats* h = current_test_stats->asserts;
    current_test_stats->asserts = as;
    as->next = h;
    as->passed = false;
    return as;
}

inline static void add_test(struct suite_stats* ss, struct test_stats* ts) {
    struct test_stats* h = ss->tests;
    ss->tests = ts;
    ts->next = h;
    ++ss->tests_total;
}

inline static void add_suite(struct suite_stats* ss) {
    struct suite_stats* h = global_stats.suites;
    global_stats.suites = ss;
    ss->next = h;
}

static int g_unit_test_last_error = 0;

#define TEST_ID(Suite, Name, Suffix) Suite ## _ ## Name ## _ ## Suffix
#define TEST_PROTOTYPE(Suite, Name) void TEST_ID(Suite, Name, run)(void)
#define TEST(Suite, Name, ...) \
TEST_PROTOTYPE(Suite, Name); \
static struct test_stats TEST_ID(Suite, Name, meta) = (struct test_stats){ \
    .name = #Name,                \
    .suite = #Suite,               \
    .id = #Name "/" #Suite,      \
    .file = __FILE__,           \
    .line = __LINE__,           \
    .run = TEST_ID(Suite, Name, run) \
};                             \
__attribute__((constructor))                               \
static void TEST_ID(Suite, Name, ctor)(void) {                             \
    register_test(&TEST_ID(Suite, Name, meta));                               \
}                              \
TEST_PROTOTYPE(Suite, Name)

// abort - помечает тест неудачным и прерывает выполнение теста
// fail - помечает тест неудачным и продолжает выполнение

#define TEST_EQ(a, b)  do { \
    struct assert_stats* _as = create_assert(); \
    if((a) != (b)) {        \
        _as->passed = true; \
        current_test_stats->test_status = TEST_STATUS_FAILED;                    \
        ++(current_test_stats->failed_asserts);   \
        printf("ERROR: %s != %s\n", #a, #b);    \
        printf("====>  " "%lld" " != " "%lld" "\n", (int64_t)(a), (int64_t)(b));  \
        g_unit_test_last_error = 1;             \
        return;                                  \
    } else {                \
        ++(current_test_stats->passed_asserts);  \
    }               \
} while(0)

inline static struct suite_stats* create_suite(const char* name) {
    for (int i = 0; i < pool_suites_next; ++i) {
        if (pool_suites[i].name == name) {
            return &pool_suites[i];
        }
    }
    assert(pool_suites_next < POOL_SUITES_MAX);
    struct suite_stats* ss = &pool_suites[pool_suites_next++];
    ss->name = name;
    add_suite(ss);
    return ss;
}

inline static void register_test(struct test_stats* test) {
    struct suite_stats* ss = create_suite(test->suite);
    add_test(ss, test);
    ++global_stats.tests_total;
}

void simple_report(FILE* f, struct global_stats* stats) {
    fprintf(f, "=================\n");
    fprintf(f, "total: %d\n", global_stats.tests_total);
    fprintf(f, "passed: %d\n", global_stats.tests_passed);
    fprintf(f, "failed: %d\n", global_stats.tests_failed + global_stats.tests_crashed);
    fprintf(f, "=================\n\n");
}

inline static int mytest_run_all(void) {
    for (struct suite_stats* ss = global_stats.suites; ss; ss = ss->next) {
        current_suite_stats = ss;
        for (struct test_stats* ts = ss->tests; ts; ts = ts->next) {
            current_test_stats = ts;
            g_unit_test_last_error = 0;
            if (ts->init) {
                ts->init();
            }
            if (ts->run) {
                ts->run();
            }
            if (ts->shutdown) {
                ts->shutdown();
            }
        }
    }
}

int mytest_main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    mytest_run_all();

    simple_report(stdout, &global_stats);
    //xml_report(stdout, &global_stats);
    fflush(stdout);

    if (global_stats.tests_failed + global_stats.tests_crashed > 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#ifndef MYTEST_NO_MAIN

int main(int argc, char** argv) {
    return mytest_main(argc, argv);
}

#endif // MYTEST_NO_MAIN

MYLIB_END_C_DECLS

#endif // MYTEST_H

/**
 * Для автоматической регистрации тестов используется простое решение из <a href="https://github.com/mortie/snow">snow</a>, где макрос генерирует точки входа с @c __attribute__((constructor))
 */

