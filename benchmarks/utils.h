#pragma once

#include <vector>
#include <random>


inline size_t max_number_of_bench_case() {
  return 4;
}

inline std::vector<double> get_random_vector(size_t bench_case) {
  std::mt19937 gen(1234 * bench_case + 65236);
  size_t size;
  switch (bench_case) {
  case 0:
    size = 128;
    break;
  case 1:
    size = 1024;
    break;
  case 2:
    size = 32 * 1024;
    break;
  case 3:
    size = 1024 * 1024;
    break;
  case 4:
    size = 8 * 1024 * 1024;
    break;
  default:
    size = 0;
    break;
  }
  std::uniform_real_distribution<double> dist(-1e9, 1e9);
  std::vector<double> data(size);
  for (size_t i = 0; i != data.size(); ++i) {
    data[i] = dist(gen);
  }
  return data;
}
