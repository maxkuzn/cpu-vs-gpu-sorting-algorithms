get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)

file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH}/*.h)
add_library(cpu_parallel STATIC ${LIB_CXX_SOURCES} ${LIB_HEADERS})
target_include_directories(cpu_parallel PUBLIC ${LIB_INCLUDE_PATH})

target_link_libraries(cpu_parallel PUBLIC OpenMP::OpenMP_CXX)

