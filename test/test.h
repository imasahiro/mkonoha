#ifndef TEST_H
#define TEST_H

struct testcase {
    int (*test) (void);
    const char *name;
};

#define TESTCASE(f) {f, #f}
int __test__(struct testcase *cases, int n)
{
    struct testcase *x;
    int i;

#define FOR_EACH_TEST(a, x, i) for(i=0, x = a; i < n; x=&a[(++i)])
    FOR_EACH_TEST(cases, x, i) {
        if (x->test() == 0) {
            fprintf(stderr, "\x1b[31m");
            fprintf(stderr, "test failed: %s\n", x->name);
        } else {
            fprintf(stderr, "\x1b[32m");
            fprintf(stderr, "test passed: %s \n", x->name);
        }
        fprintf(stderr, "\x1b[39m");
    }
    return 0;
#undef FOR_EACH_TEST
}

#endif /* end of include guard: TEST_H */
