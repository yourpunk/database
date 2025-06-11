#pragma once
 
#include <cstddef>
#include <vector>
#include <functional>
#include <atomic>
#include <thread>
#include <future>
 
template<typename row_t>
using predicate_t = std::function<bool(const row_t&)>;
 
template<typename row_t>
bool is_satisfied_for_all(const std::vector<predicate_t<row_t>>& predicates,
                          const std::vector<row_t>& data_table) {
    std::atomic<bool> result{true};
    const size_t num_threads = std::thread::hardware_concurrency();
    std::vector<std::future<void>> futures;
 
    size_t batch_size = (predicates.size() + num_threads - 1) / num_threads;
 
    for (size_t t = 0; t < num_threads; ++t) {
        size_t begin = t * batch_size;
        size_t end = std::min(begin + batch_size, predicates.size());
 
        futures.emplace_back(std::async(std::launch::async, [&, begin, end] {
            for (size_t i = begin; i < end && result.load(); ++i) {
                bool found = false;
 
                // cache-friendly: iterate rows innermost
                for (const auto& row : data_table) {
                    if (predicates[i](row)) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    result.store(false, std::memory_order_relaxed);
                    break;
                }
            }
        }));
    }
 
    for (auto& fut : futures) fut.get();
    return result.load();
}
 
template<typename row_t>
bool is_satisfied_for_any(const std::vector<predicate_t<row_t>>& predicates,
                          const std::vector<row_t>& data_table) {
    std::atomic<bool> result{false};
    const size_t num_threads = std::thread::hardware_concurrency();
    std::vector<std::future<void>> futures;
 
    size_t batch_size = (predicates.size() + num_threads - 1) / num_threads;
 
    for (size_t t = 0; t < num_threads; ++t) {
        size_t begin = t * batch_size;
        size_t end = std::min(begin + batch_size, predicates.size());
 
        futures.emplace_back(std::async(std::launch::async, [&, begin, end] {
            for (size_t i = begin; i < end && !result.load(); ++i) {
                for (const auto& row : data_table) {
                    if (predicates[i](row)) {
                        result.store(true, std::memory_order_relaxed);
                        return;
                    }
                }
            }
        }));
    }
 
    for (auto& fut : futures) fut.get();
    return result.load();
}
