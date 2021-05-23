#include "merge_sort.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <vector>



namespace gpu {
  namespace {

  template <typename T>
  void bottom_up_merge_chunks_impl(std::vector<T>& data,
                                   std::vector<T>& buffer,
                                   size_t chunk_size,
                                   size_t chunk_pair_idx) {
      
    size_t left_chunk = 2 * chunk_pair_idx;
    size_t right_chunk = 2 * chunk_pair_idx + 1;
    size_t left_idx = left_chunk * chunk_size;
    size_t left_end = left_idx + chunk_size;
    size_t right_idx = right_chunk * chunk_size;
    size_t right_end = right_idx + chunk_size;
    if (right_end > data.size()) {
      right_end = data.size();
    }
    size_t write_idx = left_chunk * chunk_size;

    while (left_idx < left_end && right_idx < right_end) {
      if (data[left_idx] < data[right_idx]) {
        buffer[write_idx++] = data[left_idx++];
      } else {
        buffer[write_idx++] = data[right_idx++];
      }
    }
    while (left_idx < left_end) {
      buffer[write_idx++] = data[left_idx++];
    }
    while (right_idx < right_end) {
      buffer[write_idx++] = data[right_idx++];
    }
  }

  template <typename T>
  void bottom_up_merge_impl(std::vector<T>& data,
                            std::vector<T>& buffer, size_t chunk_size) {
    size_t unmerged_chunks = (data.size() / chunk_size) +
                             (data.size() % chunk_size != 0);
    size_t chunk_pair_idx = 0;
    while (2 * chunk_pair_idx + 1 < unmerged_chunks) {
      bottom_up_merge_chunks_impl(data, buffer, chunk_size, chunk_pair_idx);
      ++chunk_pair_idx;
    }
  }

  template<typename T>
  __global__
  void cu_bottom_up_merge_impl(T* data, T* buffer, size_t size, size_t chunk_size) {
    // Global thread id.
    int thread_id = blockIdx.x * blockDim.x + threadIdx.x;

    size_t unmerged_chunks = (size / chunk_size) + (size % chunk_size != 0);
    size_t thread_left_chunk = 2 * thread_id;
    size_t thread_right_chunk = 2 * thread_id + 1;
    if (thread_right_chunk >= unmerged_chunks) {
      // Nothing to merge
      return;
    }
    size_t left_idx = thread_left_chunk * chunk_size;
    size_t left_end = left_idx + chunk_size;
    size_t right_idx = thread_right_chunk * chunk_size;
    size_t right_end = right_idx + chunk_size;
    if (right_end > size) {
      right_end = size;
    }

    size_t write_idx = thread_left_chunk * chunk_size;

    while (left_idx < left_end && right_idx < right_end) {
      if (data[left_idx] < data[right_idx]) {
        buffer[write_idx++] = data[left_idx++];
      } else {
        buffer[write_idx++] = data[right_idx++];
      }
    }
    while (left_idx < left_end) {
      buffer[write_idx++] = data[left_idx++];
    }
    while (right_idx < right_end) {
      buffer[write_idx++] = data[right_idx++];
    }
  }

  }  // void namespace

  template <typename T>
  void merge_sort(std::vector<T>& data) {
    constexpr size_t block_size = 1024;
    constexpr size_t merge_chunks_until_n_chunks = 64;

    T* cu_data;
    cudaMalloc(&cu_data, data.size() * sizeof(T));
    cudaMemcpy(cu_data, data.data(), data.size() * sizeof(T), cudaMemcpyHostToDevice);
    T* cu_buffer;
    cudaMalloc(&cu_buffer, data.size() * sizeof(T));


    size_t unmerged_chunks = data.size();
    size_t chunk_size = 1;
    while (unmerged_chunks > merge_chunks_until_n_chunks) {
      size_t total_thread_blocks = (unmerged_chunks / block_size) +
                                   (unmerged_chunks % block_size != 0);

      cu_bottom_up_merge_impl<T><<< total_thread_blocks, block_size >>>(
          cu_data, cu_buffer, data.size(), chunk_size);
      cudaDeviceSynchronize();
      std::swap(cu_data, cu_buffer);

      chunk_size *= 2;
      unmerged_chunks = unmerged_chunks / 2 + (unmerged_chunks % 2 != 0);
    }
    cudaMemcpy(data.data(), cu_data, data.size() * sizeof(T), cudaMemcpyDeviceToHost);
    cudaFree(cu_buffer);
    cudaFree(cu_data);

    std::vector<T> buffer(data.size());
    while (unmerged_chunks > 1) {
      bottom_up_merge_impl<T>(data, buffer, chunk_size);
      std::swap(data, buffer);

      chunk_size *= 2;
      unmerged_chunks = unmerged_chunks / 2 + (unmerged_chunks % 2 != 0);
    }
  }

  template void merge_sort<int32_t>(std::vector<int32_t>&);
  template void merge_sort<int64_t>(std::vector<int64_t>&);
  template void merge_sort<float>(std::vector<float>&);
  template void merge_sort<double>(std::vector<double>&);
  // merge_sort<int32_t>;
  // merge_sort<int64_t>;
  // merge_sort<float>;
  // merge_sort<double>;
}
