#include "gtest/gtest.h"

#include <cpu_sequential/merge_sort.h>
#include <cpu_sequential/quick_sort.h>
#include <cpu_sequential/std_sort.h>

#include <cpu_parallel/merge_sort.h>
#include <cpu_parallel/quick_sort.h>

#include <gpu/merge_sort.cuh>


#include <algorithm>
#include <gtest/gtest-typed-test.h>
#include <limits>
#include <type_traits>
#include <vector>
#include <random>


template<class T>
struct SortTestInt : public ::testing::Test {
    static std::vector<
      std::pair<
        std::function<void(std::vector<T>&)>,
        std::string
      >
    > _sort_funcs_;
};

template<class T>
struct SortTestReal : public ::testing::Test {
    static std::vector<
      std::pair<
        std::function<void(std::vector<T>&)>,
        std::string
      >
    > _sort_funcs_;
};

TYPED_TEST_SUITE_P(SortTestInt);
TYPED_TEST_SUITE_P(SortTestReal);

TYPED_TEST_P(SortTestInt, Simple) {
  std::vector<TypeParam> data{5, 1, 6, 3, 7, 3};
  for (auto&& sort_func : SortTestInt<TypeParam>::_sort_funcs_) {
    auto copy = data;
    sort_func.first(data);
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end())) << sort_func.second;
  }
}

TYPED_TEST_P(SortTestReal, Simple) {
  std::vector<TypeParam> data{5, 1, 6, 3, 7, 3};
  for (auto&& sort_func : SortTestReal<TypeParam>::_sort_funcs_) {
    auto copy = data;
    sort_func.first(data);
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end())) << sort_func.second;
  }
}

TYPED_TEST_P(SortTestInt, SmallInt) {
  std::mt19937 gen(17);
  size_t size = 1024;
  std::vector<TypeParam> data(size);
  std::uniform_int_distribution<TypeParam> dist(
      std::numeric_limits<TypeParam>::min(),
      std::numeric_limits<TypeParam>::max());
  for (size_t i = 0; i != size; ++i) {
    data[i] = dist(gen);
  }

  for (auto&& sort_func : SortTestInt<TypeParam>::_sort_funcs_) {
    auto copy = data;
    sort_func.first(data);
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end())) << sort_func.second;
  }
}

TYPED_TEST_P(SortTestReal, SmallReal) {
  std::mt19937 gen(17);
  size_t size = 1024;
  std::vector<TypeParam> data(size);
  std::uniform_real_distribution<TypeParam> dist(-1e9, 1e9);
  for (size_t i = 0; i != size; ++i) {
    data[i] = dist(gen);
  }

  for (auto&& sort_func : SortTestReal<TypeParam>::_sort_funcs_) {
    auto copy = data;
    sort_func.first(data);
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end())) << sort_func.second;
  }
}

TYPED_TEST_P(SortTestInt, BigInt) {
  std::mt19937 gen(17 + 1024);
  size_t size = 1024 * 1024;
  std::vector<TypeParam> data(size);
  std::uniform_int_distribution<TypeParam> dist(
      std::numeric_limits<TypeParam>::min(),
      std::numeric_limits<TypeParam>::max());
  for (size_t i = 0; i != size; ++i) {
    data[i] = dist(gen);
  }

  for (auto&& sort_func : SortTestInt<TypeParam>::_sort_funcs_) {
    auto copy = data;
    sort_func.first(data);
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end())) << sort_func.second;
  }
}

TYPED_TEST_P(SortTestReal, BigReal) {
  std::mt19937 gen(17 + 1024);
  size_t size = 1024 * 1024;
  std::vector<TypeParam> data(size);
  std::uniform_real_distribution<TypeParam> dist(-1e9, 1e9);
  for (size_t i = 0; i != size; ++i) {
    data[i] = dist(gen);
  }

  for (auto&& sort_func : SortTestReal<TypeParam>::_sort_funcs_) {
    auto copy = data;
    sort_func.first(data);
    ASSERT_TRUE(std::is_sorted(data.begin(), data.end())) << sort_func.second;
  }
}

REGISTER_TYPED_TEST_SUITE_P(SortTestInt,
                            Simple, SmallInt, BigInt
);

REGISTER_TYPED_TEST_SUITE_P(SortTestReal,
                            Simple, SmallReal, BigReal
);

typedef ::testing::Types<int32_t, int64_t> IntTypes;
typedef ::testing::Types<float, double> RealTypes;
INSTANTIATE_TYPED_TEST_SUITE_P(My, SortTestInt, IntTypes);
INSTANTIATE_TYPED_TEST_SUITE_P(My, SortTestReal, RealTypes);

template<typename T>
std::vector<
  std::pair<
    std::function<void(std::vector<T>&)>,
    std::string
  >
> SortTestInt<T>::_sort_funcs_{
  {cpu_sequential::merge_sort<T>, "sequential merge sort"},
  {cpu_sequential::quick_sort<T>, "sequential quick sort"},
  {cpu_sequential::std_sort_wrapper<T>, "std sort wrapper"},

  {cpu_parallel::merge_sort<T>, "parallel merge sort"},
  {cpu_parallel::quick_sort<T>, "parallel merge sort"},

  {gpu::merge_sort<T>, "GPU merge sort"},
};

template<typename T>
std::vector<
  std::pair<
    std::function<void(std::vector<T>&)>,
    std::string
  >
> SortTestReal<T>::_sort_funcs_{
  {cpu_sequential::merge_sort<T>, "sequential merge sort"},
  {cpu_sequential::quick_sort<T>, "sequential quick sort"},
  {cpu_sequential::std_sort_wrapper<T>, "std sort wrapper"},

  {cpu_parallel::merge_sort<T>, "parallel merge sort"},
  {cpu_parallel::quick_sort<T>, "parallel merge sort"},
 
  {gpu::merge_sort<T>, "GPU merge sort"},
};

