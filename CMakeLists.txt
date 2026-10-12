cmake_minimum_required(VERSION 4.2)
project(variation_aware_circuit_optimizer_cpp)

set(CMAKE_CXX_STANDARD 20)

# Main executable
add_executable(variation_aware_circuit_optimizer_cpp
        main.cpp
        circuit.cpp
        simulator.cpp
        variation.cpp
        analysis.cpp
        objective.cpp
        optimizer.cpp
        scipy_optimizer.cpp
)

target_include_directories(
        variation_aware_circuit_optimizer_cpp
        PRIVATE
        /opt/homebrew/opt/eigen@3/include/eigen3
        ${CMAKE_SOURCE_DIR}/external/LBFGSpp/include
)

# Comparison executable
add_executable(comparison
         comparison.cpp
        circuit.cpp
        simulator.cpp
        variation.cpp
        analysis.cpp
        objective.cpp
        optimizer.cpp
        scipy_optimizer.cpp
)

target_include_directories(
        comparison
        PRIVATE
        /opt/homebrew/opt/eigen@3/include/eigen3
        ${CMAKE_SOURCE_DIR}/external/LBFGSpp/include
)

# Enable testing
enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

# Circuit tests
add_executable(
        test_circuit
        tests/test_circuit.cpp
        circuit.cpp
)

target_include_directories(
        test_circuit
        PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
        test_circuit
        PRIVATE
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(test_circuit)
add_executable(
        test_simulator
        tests/test_simulator.cpp
        simulator.cpp
        circuit.cpp
)

target_include_directories(
        test_simulator
        PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
        test_simulator
        PRIVATE
        GTest::gtest_main
)

gtest_discover_tests(test_simulator)

# Variation tests
add_executable(
        test_variation
        tests/test_variation.cpp
        variation.cpp
        circuit.cpp
)

target_include_directories(
        test_variation
        PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
        test_variation
        PRIVATE
        GTest::gtest_main
)

gtest_discover_tests(test_variation)

# Analysis tests
add_executable(
        test_analysis
        tests/test_analysis.cpp
        analysis.cpp
        simulator.cpp
        circuit.cpp
)

target_include_directories(
        test_analysis
        PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
        test_analysis
        PRIVATE
        GTest::gtest_main
)

gtest_discover_tests(test_analysis)

# Objective tests
add_executable(
        test_objective
        tests/test_objective.cpp
        objective.cpp
)

target_include_directories(
        test_objective
        PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
        test_objective
        PRIVATE
        GTest::gtest_main
)

gtest_discover_tests(test_objective)

# Optimizer tests
add_executable(
        test_optimizer
        tests/test_optimizer.cpp
        optimizer.cpp
        objective.cpp
        analysis.cpp
        variation.cpp
        simulator.cpp
        circuit.cpp
)

target_include_directories(
        test_optimizer
        PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
        test_optimizer
        PRIVATE
        GTest::gtest_main
)

gtest_discover_tests(test_optimizer)

# Scipy optimizer tests
add_executable(
        test_scipy_optimizer
        tests/test_scipy_optimizer.cpp
        scipy_optimizer.cpp
        optimizer.cpp
        objective.cpp
        analysis.cpp
        variation.cpp
        simulator.cpp
        circuit.cpp
)

target_include_directories(
        test_scipy_optimizer
        PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/LBFGSpp/include
        /opt/homebrew/opt/eigen@3/include/eigen3
)

target_link_libraries(
        test_scipy_optimizer
        PRIVATE
        GTest::gtest_main
)

gtest_discover_tests(test_scipy_optimizer)