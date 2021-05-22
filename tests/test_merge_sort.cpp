#include "gtest/gtest.h"

#include <cpu_sequential/merge_sort.h>

#include <algorithm>
#include <limits>
#include <vector>
#include <random>


TEST(merge_sort, simple) {
  std::vector<int> data{5, 1, 6, 3, 7, 3, 7};

  merge_sort(data);

  EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
  for (size_t i = 0; i + 1 < data.size(); ++i) {
    ASSERT_LE(data[i], data[i + 1]);
  }
}

TEST(merge_sort, random_small) {
  std::mt19937 gen(17);
  std::uniform_int_distribution<int> dist(
      std::numeric_limits<int>::min(),
      std::numeric_limits<int>::max());

  const size_t size = 1024;
  std::vector<int> data(size);

  for (size_t i = 0; i != data.size(); ++i) {
    data[i] = dist(gen);
  }

  merge_sort(data);
  EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
  for (size_t i = 0; i + 1 < data.size(); ++i) {
    ASSERT_LE(data[i], data[i + 1]);
  }
}

