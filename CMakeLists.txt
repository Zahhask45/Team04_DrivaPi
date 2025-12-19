cmake_minimum_required(VERSION 3.14)
project(DrivaPi_MotorSensor C CXX)

# Use C++17 for production code; tests can be C (Unity) or C++ as needed
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option: enable host-side unit tests (Unity/CMock style) – OFF by default
option(DRIVAPI_ENABLE_TESTS "Build host-side unit tests (Unity/CMock)" ON)

# Production sources (shared with firmware and host builds)
add_subdirectory(src)

if(DRIVAPI_ENABLE_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()
