/**
 * =====================================
 * Logical Query Evaluation Benchmark
 * =====================================
 *
 * This program evaluates the performance and correctness of two types of logical queries
 * over a simulated table of data:
 * 
 * - is_satisfied_for_all(...) : true if every predicate matches at least one row
 * - is_satisfied_for_any(...) : true if any predicate matches at least one row
 *
 * Tests are run both for expected-true and expected-false queries.
 * OpenMP parallelism is used to speed up evaluation.
 * 
 * You can change data generation parameters in `params.h`.
 */

#include "tests.h"

#include <chrono>
#include <cstdio>
#include <omp.h>

template<class Test>
void eval(const char* test_name) {
    Test test;

    try {
        auto begin = std::chrono::steady_clock::now();
        test.run_test();
        auto end = std::chrono::steady_clock::now();

        if (!test.verify()) {
            printf("%s       --- wrong result ---\n", test_name);
        } else {
            auto ms = duration_cast<std::chrono::milliseconds>(end - begin);
            printf("%s          %7ldms\n", test_name, ms.count());
        }
    } catch (...) {
        printf("%s      --- not implemented ---\n", test_name);
    }
}

int main() {
    if (!omp_get_cancellation()) {
        printf("-----------------------------------------------------------------------------\n");
        printf("| WARNING: OpenMP cancellations are not enabled                             |\n");
        printf("| You can enable them by setting environment variable OMP_CANCELLATION=true |\n");
        printf("-----------------------------------------------------------------------------\n");
    }

    // Test: are all predicates satisfied by at least one row?
    eval<TestAll<true>>("true = is_satisfied_for_all(...)");

    // Test: is any predicate satisfied by at least one row?
    eval<TestAny<true>>("true = is_satisfied_for_any(...)");

    printf("\n");

    // Negative test cases (should return false)
    eval<TestAll<false>>("false = is_satisfied_for_all(...)");
    eval<TestAny<false>>("false = is_satisfied_for_any(...)");

    return 0;
}
