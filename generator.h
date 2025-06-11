#pragma once

#include "query.h"

#include <cstddef>
#include <utility>
#include <vector>

// Simulated database row type
using test_row_t = int;

/**
 * Generates a table and a query using logical AND (conjunction).
 * 
 * @param expected - if true, the generated query will match the data (evaluate to true)
 */
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>> generate_all(bool expected);

/**
 * Generates a table and a query using logical OR (disjunction).
 * 
 * @param expected - if true, the generated query will match the data (evaluate to true)
 */
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>> generate_any(bool expected);

// Logical operation type used to combine predicates
enum Operation {
    conjunction,
    disjunction
};

/**
 * General-purpose instance generator.
 *
 * @param operation               - logical operation to combine predicates (AND/OR)
 * @param is_query_evaluated_true - should the query evaluate to true on the generated data?
 * @param length_of_query         - number of predicates in the query
 * @param count_of_rows           - number of rows in the generated table
 */
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_instance_with_query(Operation operation, bool is_query_evaluated_true, size_t length_of_query,
                             size_t count_of_rows);
