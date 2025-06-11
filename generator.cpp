#include "generator.h"
#include "params.h"

#include <random>
#include <chrono>
#include <algorithm>
#include <thread>

// Global random number generator (Mersenne Twister)
std::mt19937 rng(SEED); // NOLINT(*-msc51-cpp)

// Simulated predicate check — artificial delay (~2 microseconds)
void predicate_checking_simulation_function() {
    auto begin = std::chrono::steady_clock::now();
    while (true) {
        auto now = std::chrono::steady_clock::now();
        if (duration_cast<std::chrono::microseconds>(now - begin).count() >= 2L) break;
    }
}

// Fills a boolean vector with values based on a probability,
// and guarantees at least one occurrence of a specific boolean value
void fill_vector_by_bools_by_ppt(const double ppt_of_true, std::vector<bool>& to_fill, const bool bool_value_to_ensure) {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    bool ensured = false;
    auto size = to_fill.size();

    for (size_t i = 0; i < size - 1; ++i) {
        bool is_true = dis(rng) < ppt_of_true;
        if (!ensured && is_true == bool_value_to_ensure) {
            ensured = true;
        }
        to_fill[i] = is_true;
    }

    to_fill[size - 1] = ensured ? (dis(rng) < ppt_of_true) : bool_value_to_ensure;
}

// Generates a dataset and predicates with a mix of true/false predicates
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_instance_with_query(size_t length_of_query, size_t count_of_rows, bool bool_value_to_ensure, double probability) {
    if (length_of_query > count_of_rows) {
        throw std::invalid_argument("length_of_query must be <= count_of_rows");
    }

    std::vector<bool> is_predicate_true(length_of_query);
    fill_vector_by_bools_by_ppt(probability, is_predicate_true, bool_value_to_ensure);

    std::vector<test_row_t> data(count_of_rows);
    std::iota(data.begin(), data.end(), 0);

    std::vector<size_t> indexes(count_of_rows);
    std::iota(indexes.begin(), indexes.end(), 0);
    std::shuffle(indexes.begin(), indexes.end(), rng);

    std::uniform_int_distribution<test_row_t> dist(1, 20);
    std::vector<predicate_t<test_row_t>> predicates(length_of_query);

    for (size_t i = 0; i < length_of_query; ++i) {
        test_row_t value = dist(rng) + static_cast<test_row_t>(count_of_rows / std::thread::hardware_concurrency());
        if (!is_predicate_true[i]) value = -1;

        predicates[i] = [value](const test_row_t& input) {
            predicate_checking_simulation_function();
            return input == value;
        };
    }

    return {data, predicates};
}

// Generates a conjunction (AND) query that always evaluates to true
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_instance_with_query_conjunction_t(size_t length_of_query, size_t count_of_rows) {
    std::vector<test_row_t> data(count_of_rows);
    std::iota(data.begin(), data.end(), 0);
    std::shuffle(data.begin(), data.end(), rng);

    std::vector<predicate_t<test_row_t>> predicates(length_of_query);

    for (size_t i = 0; i < length_of_query; ++i) {
        std::uniform_int_distribution<test_row_t> dis_index(0, static_cast<test_row_t>((count_of_rows - 1) / 20));
        auto start = dis_index(rng);
        auto end = static_cast<test_row_t>(count_of_rows - dis_index(rng));

        std::uniform_int_distribution<test_row_t> dis_step(2, 5);
        auto step = dis_step(rng);

        predicates[i] = [start, end, step](const test_row_t& value) {
            predicate_checking_simulation_function();
            return value >= start && value <= end && value % step == 0;
        };
    }

    return {data, predicates};
}

// Generates a disjunction (OR) query that always evaluates to false
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_instance_with_query_disjunction_f(size_t length_of_query, size_t count_of_rows) {
    std::vector<int> data(count_of_rows);
    std::iota(data.begin(), data.end(), 0);
    std::shuffle(data.begin(), data.end(), rng);

    std::vector<predicate_t<test_row_t>> predicates(length_of_query);

    for (size_t i = 0; i < length_of_query; ++i) {
        predicates[i] = [](const test_row_t& value) {
            predicate_checking_simulation_function();
            return value == -1;
        };
    }

    return {data, predicates};
}

// Main generator dispatcher
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_instance_with_query(Operation operation, bool expected, size_t length_of_query, size_t count_of_rows) {
    rng.seed(SEED); // Reset RNG for reproducibility

    switch (operation) {
        case conjunction:
            return expected
                ? generate_instance_with_query_conjunction_t(length_of_query, count_of_rows)
                : generate_instance_with_query(length_of_query, count_of_rows, false, t_probability_conjunction_predicate);
        case disjunction:
            return expected
                ? generate_instance_with_query(length_of_query, count_of_rows, true, t_probability_disjunction_predicate)
                : generate_instance_with_query_disjunction_f(length_of_query, count_of_rows);
    }

    throw std::invalid_argument("Unsupported operation");
}

// Wrapper for conjunction generator
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_all(bool expected) {
    return expected
        ? generate_instance_with_query(conjunction, true, length_of_query_all_true, count_of_rows_all_true)
        : generate_instance_with_query(conjunction, false, length_of_query_all_false, count_of_rows_all_false);
}

// Wrapper for disjunction generator
std::pair<std::vector<test_row_t>, std::vector<predicate_t<test_row_t>>>
generate_any(bool expected) {
    return expected
        ? generate_instance_with_query(disjunction, true, length_of_query_any_true, count_of_rows_any_true)
        : generate_instance_with_query(disjunction, false, length_of_query_any_false, count_of_rows_any_false);
}
