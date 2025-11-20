#
# Usage:
# cmake -S . -B build -G Ninja
# cmake --build build
# ctest --test-dir build

cmake_minimum_required(VERSION 3.16)

project(wifiequ_linux LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(WIFIEQU_BUILD_ROOT "${CMAKE_CURRENT_BINARY_DIR}/artifacts")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${WIFIEQU_BUILD_ROOT}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${WIFIEQU_BUILD_ROOT}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${WIFIEQU_BUILD_ROOT}/bin")

set(WIFIEQU_LOG_DIR "${WIFIEQU_BUILD_ROOT}/logs")

if(NOT TARGET libdmotservices)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../../libdmotservices/c/linux"
                     libdmotservices_for_wifiequ)
endif()

include(CTest)

find_library(MICROHTTPD_LIBRARY NAMES microhttpd REQUIRED)
find_library(NL3_LIBRARY NAMES nl-3 REQUIRED)
find_library(NLGENL3_LIBRARY NAMES nl-genl-3 REQUIRED)
find_library(CMOCKA_LIBRARY NAMES cmocka)

if(NOT CMOCKA_LIBRARY AND BUILD_TESTING)
    message(FATAL_ERROR "cmocka library is required to build wifiequ tests.")
endif()

set(WIFIEQU_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
file(GLOB_RECURSE WIFIEQU_SOURCES "${WIFIEQU_SRC_DIR}/*.c")
set(WIFIEQU_MAIN_SOURCE "${CMAKE_CURRENT_LIST_DIR}/src/wifiequd.c")
list(REMOVE_ITEM WIFIEQU_SOURCES "${WIFIEQU_MAIN_SOURCE}")

add_library(wifiequ_core STATIC ${WIFIEQU_SOURCES})
target_include_directories(wifiequ_core
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include"
        "${CMAKE_CURRENT_LIST_DIR}/../../libdmotservices/c/linux/include"
        "/usr/include/libnl3"
)
target_compile_options(wifiequ_core PUBLIC -O2 -g -Wall -Wextra)
target_compile_definitions(wifiequ_core PUBLIC _GNU_SOURCE)
target_link_libraries(wifiequ_core
    PUBLIC
        libdmotservices
        "${MICROHTTPD_LIBRARY}"
        "${NL3_LIBRARY}"
        "${NLGENL3_LIBRARY}"
        m
)

set(WIFIEQU_LINK_LIBS
    wifiequ_core
    "${MICROHTTPD_LIBRARY}"
    "${NL3_LIBRARY}"
    "${NLGENL3_LIBRARY}"
    m
)

add_executable(wifiequd "${WIFIEQU_MAIN_SOURCE}")
target_link_libraries(wifiequd PRIVATE ${WIFIEQU_LINK_LIBS})

add_custom_target(wifiequ_logs ALL
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${WIFIEQU_LOG_DIR}")

if(BUILD_TESTING)
    file(GLOB WIFIEQU_TEST_SOURCES "${CMAKE_CURRENT_LIST_DIR}/tests/*.c")
    foreach(test_src IN LISTS WIFIEQU_TEST_SOURCES)
        get_filename_component(test_name "${test_src}" NAME_WE)
        set(test_target "wifiequ_${test_name}")
        add_executable("${test_target}" "${test_src}")
        target_link_libraries("${test_target}"
            PRIVATE
                ${WIFIEQU_LINK_LIBS}
                "${CMOCKA_LIBRARY}"
        )
        add_test(NAME "${test_target}" COMMAND "${test_target}")
    endforeach()
endif()

option(WIFIEQU_BUILD_EXAMPLES "Build the WiFiEqu example executables." ON)

set(WIFIEQU_EXAMPLE_TARGETS "")
if(WIFIEQU_BUILD_EXAMPLES)
    file(GLOB WIFIEQU_EXAMPLE_SOURCES "${CMAKE_CURRENT_LIST_DIR}/examples/*.c")
    foreach(ex_src IN LISTS WIFIEQU_EXAMPLE_SOURCES)
        get_filename_component(ex_name "${ex_src}" NAME_WE)
        add_executable("${ex_name}" "${ex_src}")
        target_link_libraries("${ex_name}" PRIVATE ${WIFIEQU_LINK_LIBS})
        list(APPEND WIFIEQU_EXAMPLE_TARGETS "${ex_name}")
    endforeach()
endif()

if(WIFIEQU_EXAMPLE_TARGETS)
    add_custom_target(wifiequ_linux_examples DEPENDS ${WIFIEQU_EXAMPLE_TARGETS})

    add_custom_target(wifiequ_linux_example_demo
        DEPENDS wifiequ_linux_examples
        COMMENT "Executing WiFiEqu example binaries")

    foreach(ex IN LISTS WIFIEQU_EXAMPLE_TARGETS)
        add_custom_command(TARGET wifiequ_linux_example_demo
            COMMAND "${CMAKE_COMMAND}" -E echo "Running: ${ex}"
            COMMAND "$<TARGET_FILE:${ex}>")
    endforeach()
else()
    add_custom_target(wifiequ_linux_examples
        COMMAND "${CMAKE_COMMAND}" -E echo "No WiFiEqu examples were found.")
    add_custom_target(wifiequ_linux_example_demo
        COMMAND "${CMAKE_COMMAND}" -E echo "No WiFiEqu examples to run.")
endif()
