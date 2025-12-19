# Host-side unit tests wrapper (runs Ceedling test suite via script)

add_executable(unit_tests
    host/test_placeholder.c
)

add_test(NAME unit_tests 
    COMMAND unit_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

set_tests_properties(unit_tests PROPERTIES
    ENVIRONMENT "COVERAGE_DIR=${CMAKE_SOURCE_DIR}/build/coverage"
    LABELS "ceedling"
)


# GoogleTest-based C++ tests for motor speed
# Fetch GoogleTest if not already available
include(FetchContent)
if(NOT TARGET GTest::gtest_main)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    # On Linux this is not required, but harmless; mainly for MSVC
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Create the test executable
add_executable(motor_speed_test
    unit/test_motor_speed.cpp
)

# Link our test executable against our library and GoogleTest
target_link_libraries(motor_speed_test
    PRIVATE
        motor_sensor_lib
        GTest::gtest_main
)

# Add the test to CTest (for the 'ctest' command)
include(GoogleTest)
gtest_discover_tests(motor_speed_test
    PROPERTIES LABELS "gtest"
)

