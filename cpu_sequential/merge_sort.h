#pragma once

#include <vector>


namespace {
  template <typename T>
  void merge_impl(std::vector<T>& data, std::vector<T>& buffer,
                  size_t begin, size_t middle, size_t end) {
    size_t left_idx = begin;
    size_t right_idx = middle;
    size_t out_idx = begin;
    while (left_idx != middle && right_idx != end) {
      if (data[left_idx] < data[right_idx]) {
        buffer[out_idx++] = data[left_idx++];
      } else {
        buffer[out_idx++] = data[right_idx++];
      }
    }
    while (left_idx != middle) {
      buffer[out_idx++] = data[left_idx++];
    }
    while (right_idx != end) {
      buffer[out_idx++] = data[right_idx++];
    }

    // Copy data back
    for (size_t i = begin; i != end; ++i) {
      data[i] = buffer[i];
    }
  }

  template <typename T>
  void merge_sort_impl(std::vector<T>& data, std::vector<T>& buffer,
                       size_t begin, size_t end) {
    if (end - begin <= 1) {
      return;
    }

    size_t middle = begin + (end - begin) / 2;
    merge_sort_impl(data, buffer, begin, middle);
    merge_sort_impl(data, buffer, middle, end);
    merge_impl(data, buffer, begin, middle, end);
  }
}

namespace cpu_sequential {

  template <typename T>
  void merge_sort(std::vector<T>& data) {
    std::vector<T> buffer(data.size());
    merge_sort_impl(data, buffer, 0, data.size());
  }

}
