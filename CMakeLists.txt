cmake_minimum_required(VERSION 3.12)

# set the project name
project(cpu_vs_gpu_sorting_algorithms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# SET(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
# SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
# SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
# SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")

add_compile_options(-Wall -Wextra -Werror)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")


OPTION (USE_OpenMP "Use OpenMP to enamble <omp.h>" ON)

# Find OpenMP
if(APPLE AND USE_OpenMP)
    set(CMAKE_C_COMPILER "/usr/local/Cellar/llvm/12.0.0_1/bin/clang")
    set(CMAKE_CXX_COMPILER "/usr/local/Cellar/llvm/12.0.0_1/bin/clang++")
    set(OPENMP_LIBRARIES "/usr/local/Cellar/llvm/12.0.0_1/lib")
    set(OPENMP_INCLUDES "/usr/local/Cellar/llvm/12.0.0_1/include")

    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(OpenMP_C "${CMAKE_C_COMPILER}")
        set(OpenMP_C_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
        set(OpenMP_C_LIB_NAMES "libomp")
        # set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        set(OpenMP_libomp_LIBRARY omp)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
        set(OpenMP_CXX_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
        set(OpenMP_CXX_LIB_NAMES "libomp")
        set(OpenMP_libomp_LIBRARY omp)
      # set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    endif()
endif()

if(USE_OpenMP)
  find_package(OpenMP REQUIRED)
endif(USE_OpenMP)

if (OPENMP_FOUND)
    include_directories("${OPENMP_INCLUDES}")
    link_directories("${OPENMP_LIBRARIES}")
    # set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    # set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    # set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif(OPENMP_FOUND)



add_subdirectory(cpu_sequential)
add_subdirectory(cpu_parallel)

add_subdirectory(tests)
add_subdirectory(benchmarks)
