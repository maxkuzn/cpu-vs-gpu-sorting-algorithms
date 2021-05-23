#include <benchmark/benchmark.h>
#include <random>
#include <vector>
#include <chrono>
#include <functional>

#include "utils.h"

#include <cpu_sequential/merge_sort.h>
#include <cpu_sequential/quick_sort.h>
#include <cpu_sequential/std_sort.h>

#include <cpu_parallel/merge_sort.h>
#include <cpu_parallel/quick_sort.h>

#include <gpu/merge_sort.cuh>

static void BM_Sort(benchmark::State& state,
            const std::function<void(std::vector<double>&)>& sort_func) {
  std::vector<double> data = get_random_vector(state.range(0));

  for (auto _ : state) {
    state.PauseTiming();
    auto copy = data;
    state.ResumeTiming();
    sort_func(copy);
  }
  state.counters["size"] = data.size();
}



// Register the function as a benchmark
// ============= Sequential algorithms ===============
BENCHMARK_CAPTURE(BM_Sort, MergeSort, cpu_sequential::merge_sort<double>)
  ->DenseRange(0, max_number_of_bench_case(), 1)
  ->Unit(benchmark::kSecond);
  // ->UseManualTime();

BENCHMARK_CAPTURE(BM_Sort, QuickSort, cpu_sequential::quick_sort<double>)
  ->DenseRange(0, max_number_of_bench_case(), 1)
  ->Unit(benchmark::kSecond);

BENCHMARK_CAPTURE(BM_Sort, StdSort, cpu_sequential::std_sort_wrapper<double>)
  ->DenseRange(0, max_number_of_bench_case(), 1)
  ->Unit(benchmark::kSecond);


// OpenMP
// =============== OpenMP algorithms =================
BENCHMARK_CAPTURE(BM_Sort, ParallelMergeSort, cpu_parallel::merge_sort<double>)
  ->DenseRange(0, max_number_of_bench_case(), 1)
  ->Unit(benchmark::kSecond);

BENCHMARK_CAPTURE(BM_Sort, ParallelQuickSort, cpu_parallel::quick_sort<double>)
  ->DenseRange(0, max_number_of_bench_case(), 1)
  ->Unit(benchmark::kSecond);


// ================ CUDA algorithms ==================
BENCHMARK_CAPTURE(BM_Sort, CUDAMergeSort, gpu::merge_sort<double>)
  ->DenseRange(0, max_number_of_bench_case(), 1)
  ->Unit(benchmark::kSecond);

// Run the benchmark
BENCHMARK_MAIN();

