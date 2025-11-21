#
# Usage:
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build --target txrxcli
#   ctest --test-dir build/linux

cmake_minimum_required(VERSION 3.15)
project(txrxcli-linux LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(txrxcli
    src/main.cpp
    src/protocol.cpp
    src/server/server.cpp
)

target_include_directories(txrxcli PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

option(TXRXCLI_BUILD_TESTS "Build txrxcli tests" ON)
if (TXRXCLI_BUILD_TESTS)
    enable_testing()
    find_package(GTest)
    find_package(spdlog)
    if (GTest_FOUND AND spdlog_FOUND)
        add_executable(txrxcli_tests
            tests/protocol_tests.cpp
            src/protocol.cpp
        )
        target_include_directories(txrxcli_tests PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/include
            ${GTEST_INCLUDE_DIRS}
        )
        target_link_libraries(txrxcli_tests PRIVATE
            GTest::GTest
            GTest::Main
            spdlog::spdlog
        )
        add_test(NAME protocol_tests COMMAND txrxcli_tests)
    else()
        message(STATUS "GTest or spdlog not found; skipping txrxcli_tests target")
    endif()
endif()
