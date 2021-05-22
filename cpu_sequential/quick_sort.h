#pragma once

#include <random>
#include <vector>
#include <queue>
#include <tuple>


namespace {
  template <typename T>
  size_t partition_impl(std::vector<T>& data, size_t begin, size_t end,
                      T pivot, bool pivot_to_left) {
    size_t left = begin;
    size_t right = end - 1;
    while (left < right) {
      while (left < end &&
             (data[left] < pivot ||
              (pivot_to_left && data[left] == pivot))) {
        ++left;
      }
      while (right > 0 &&
             (data[right] > pivot ||
              (!pivot_to_left && data[right] == pivot))) {
        --right;
      }
      if (left < right) {
        std::swap(data[left], data[right]);
      }
    }
    return left;
  }

  template <typename T>
  void quick_sort_impl(std::vector<T>& data) {
    static std::mt19937 gen(42);

    // Use queue to get rid of recursion
    std::queue<std::tuple<size_t, size_t>> q;
    q.push({0, data.size()});
    while (!q.empty()) {
      auto [begin, end] = q.front();
      q.pop();
      if (end - begin <= 1) {
        continue;
      }
      std::uniform_int_distribution<size_t> dist(begin, end - 1);
      T pivot = data[dist(gen)];
      size_t right_begin = partition_impl(data, begin, end, pivot, true);
      size_t left_end = partition_impl(data, begin, right_begin, pivot, false);
      q.push({begin, left_end});
      q.push({right_begin, end});
    }
  }
}

namespace cpu_sequential {

  template <typename T>
  void quick_sort(std::vector<T>& data) {
    quick_sort_impl(data);
  }

}
