# Ceedling unit tests (motor_servo + speed_sensor)
add_test(NAME ceedling_tests
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/unit/run_all_tests.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/unit
)

set_tests_properties(ceedling_tests PROPERTIES
    ENVIRONMENT "COVERAGE_DIR=${CMAKE_SOURCE_DIR}/build/coverage"
    LABELS "ceedling"
)

# Ensure GoogleTest is available
include(FetchContent)
if(NOT TARGET GTest::gtest_main)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Create the GTest executable
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
