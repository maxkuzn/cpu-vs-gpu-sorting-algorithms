# Google test setup
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



enable_testing()


macro(package_add_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    target_link_libraries(${TEST_NAME} gtest gtest_main cpu_sequential)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_target_properties(${TEST_NAME} PROPERTIES FOLDER tests)
endmacro()


file(GLOB TEST_SRC_FILES "test_*.cpp")

foreach(TEST_SRC_FILE ${TEST_SRC_FILES})
    get_filename_component(TEST_NAME ${TEST_SRC_FILE} NAME_WE)
    package_add_test(${TEST_NAME} ${TEST_SRC_FILE})
endforeach()

add_executable(all_tests ${TEST_SRC_FILES})
target_link_libraries(all_tests gtest gtest_main cpu_sequential)
add_test(NAME all_tests COMMAND all_tests)
set_target_properties(all_tests PROPERTIES FOLDER tests)

