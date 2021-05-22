get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)


file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH}/*.h)
add_library(cpu_sequential STATIC ${LIB_CXX_SOURCES} ${LIB_HEADERS})
target_include_directories(cpu_sequential PUBLIC ${LIB_INCLUDE_PATH})

