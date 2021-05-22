#include <benchmark/benchmark.h>
#include <random>
#include <vector>
#include <chrono>

#include "utils.h"

#include <cpu_sequential/merge_sort.h>
#include <cpu_sequential/quick_sort.h>
#include <cpu_sequential/std_sort.h>


static void BM_Sort(benchmark::State& state,
            const std::function<void(std::vector<double>&)>& sort_func) {
  std::vector<double> data = get_random_vector(state.range(0));

  for (auto _ : state) {
    auto copy = data;
    auto start = std::chrono::high_resolution_clock::now();
    sort_func(copy);
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(
        end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
  state.counters["size"] = data.size();
}



// Register the function as a benchmark
BENCHMARK_CAPTURE(BM_Sort, MergeSort, cpu_sequential::merge_sort<double>)
  ->DenseRange(0, max_number_of_bench_case(), 1)
  ->Unit(benchmark::kSecond)
  ->UseManualTime();

BENCHMARK_CAPTURE(BM_Sort, QuickSort, cpu_sequential::quick_sort<double>)
  ->DenseRange(0, max_number_of_bench_case(), 1)
  ->Unit(benchmark::kSecond)
  ->UseManualTime();

BENCHMARK_CAPTURE(BM_Sort, StdSort, cpu_sequential::std_sort_wrapper<double>)
  ->DenseRange(0, max_number_of_bench_case(), 1)
  ->Unit(benchmark::kSecond)
  ->UseManualTime();


// Run the benchmark
BENCHMARK_MAIN();

