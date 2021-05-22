#include "gtest/gtest.h"

#include <cpu_sequential/merge_sort.h>
#include <cpu_sequential/quick_sort.h>
#include <cpu_sequential/std_sort.h>

#include <algorithm>
#include <gtest/gtest-typed-test.h>
#include <limits>
#include <type_traits>
#include <vector>
#include <random>


template<class T>
struct SortTest : public ::testing::Test {
    static std::vector<
      std::pair<
        std::function<void(std::vector<T>&)>,
        std::string
      >
    > _sort_funcs_;
};

TYPED_TEST_SUITE_P(SortTest);

TYPED_TEST_P(SortTest, Simple) {
  std::vector<TypeParam> data{5, 1, 6, 3, 7, 3};
  for (auto&& sort_func : SortTest<TypeParam>::_sort_funcs_) {
    auto copy = data;
    sort_func.first(data);
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end())) << sort_func.second;
  }
}

TYPED_TEST_P(SortTest, Small) {
  std::mt19937 gen(17);
  size_t size = 1024;
  std::vector<TypeParam> data(size);
  if (std::is_integral_v<TypeParam>) {
    std::uniform_int_distribution<TypeParam> dist(
        std::numeric_limits<TypeParam>::min(),
        std::numeric_limits<TypeParam>::max());
    for (size_t i = 0; i != size; ++i) {
      data[i] = dist(gen);
    }
  } else {
    std::uniform_real_distribution<TypeParam> dist(-1e9, 1e9);
    for (size_t i = 0; i != size; ++i) {
      data[i] = dist(gen);
    }
  }

  for (auto&& sort_func : SortTest<TypeParam>::_sort_funcs_) {
    auto copy = data;
    sort_func.first(data);
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end())) << sort_func.second;
  }
}

TYPED_TEST_P(SortTest, Big) {
  std::mt19937 gen(17 + 1024);
  size_t size = 1024 * 1024;
  std::vector<TypeParam> data(size);
  if (std::is_integral_v<TypeParam>) {
    std::uniform_int_distribution<TypeParam> dist(
        std::numeric_limits<TypeParam>::min(),
        std::numeric_limits<TypeParam>::max());
    for (size_t i = 0; i != size; ++i) {
      data[i] = dist(gen);
    }
  } else {
    std::uniform_real_distribution<TypeParam> dist(-1e9, 1e9);
    for (size_t i = 0; i != size; ++i) {
      data[i] = dist(gen);
    }
  }

  for (auto&& sort_func : SortTest<TypeParam>::_sort_funcs_) {
    auto copy = data;
    sort_func.first(data);
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end())) << sort_func.second;
  }
}


REGISTER_TYPED_TEST_SUITE_P(SortTest,
                            Simple, Small, Big
);

typedef ::testing::Types<int32_t, int64_t, float, double> MyTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(My, SortTest, double);

template<typename T>
std::vector<
  std::pair<
    std::function<void(std::vector<T>&)>,
    std::string
  >
> SortTest<T>::_sort_funcs_{
  {cpu_sequential::merge_sort<T>, "sequential merge sort"},
  {cpu_sequential::quick_sort<T>, "sequential quick sort"},
  {cpu_sequential::std_sort_wrapper<T>, "std sort wrapper"},
};

