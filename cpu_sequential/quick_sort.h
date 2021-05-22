#pragma once

#include <random>
#include <vector>
#include <queue>
#include <tuple>


namespace cpu_sequential {

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
  void quick_sort_impl(std::vector<T>& data, size_t begin, size_t end) {
    static std::mt19937 gen(42);
    if (end - begin <= 1) {
      return;
    }
    std::uniform_int_distribution<size_t> dist(begin, end - 1);
    T pivot = data[dist(gen)];
    size_t right_begin = partition_impl(data, begin, end, pivot, true);
    size_t left_end = partition_impl(data, begin, right_begin, pivot, false);
    quick_sort_impl(data, begin, left_end);
    quick_sort_impl(data, right_begin, end);
  }

  }  // void namespace

  template <typename T>
  void quick_sort(std::vector<T>& data) {
    quick_sort_impl(data, 0, data.size());
  }

}
