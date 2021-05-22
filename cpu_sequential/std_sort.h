#include <vector>
#include <algorithm>

namespace cpu_sequential {

  template <typename T>
  void std_sort_wrapper(std::vector<T>& data) {
    std::sort(data.begin(), data.end());
  }

}

