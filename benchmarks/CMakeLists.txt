# include(FetchContent)
# FetchContent_Declare(googlebenchmark
#                      GIT_REPOSITORY https://github.com/google/benchmark
#         )
# FetchContent_MakeAvailable(googlebenchmark)

add_subdirectory("${PROJECT_SOURCE_DIR}/libs/gbenchmark" "libs/banchmark")



# Find benchmark sources
file(GLOB BENCH_SOURCES "bench_*.cpp")

# Build & run benchmarks
foreach(BENCH_SOURCE ${BENCH_SOURCES})
    get_filename_component(BENCH_NAME ${BENCH_SOURCE} NAME_WE)
    set(BENCH_TARGET "${BENCH_NAME}")

    add_executable(${BENCH_TARGET} ${BENCH_SOURCE})
    target_link_libraries(${BENCH_TARGET} benchmark
                          cpu_sequential cpu_parallel)
endforeach()

