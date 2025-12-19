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
)