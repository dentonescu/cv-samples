#
# Usage:
# cmake -S . -B build -G Ninja
# cmake --build build
# ctest --test-dir build

cmake_minimum_required(VERSION 3.16)

file(READ "${CMAKE_CURRENT_LIST_DIR}/../VERSION" LIBDMOTSERVICES_C_VERSION_RAW)
string(STRIP "${LIBDMOTSERVICES_C_VERSION_RAW}" LIBDMOTSERVICES_C_VERSION)

project(libdmotservices_c
        VERSION "${LIBDMOTSERVICES_C_VERSION}"
        LANGUAGES C)

include(CTest)

option(LIBDMOTSERVICES_C_BUILD_TESTING "Enable libdmotservices C tests" ON)
option(LIBDMOTSERVICES_C_BUILD_EXAMPLES "Enable libdmotservices C examples" ON)

if(LIBDMOTSERVICES_C_BUILD_EXAMPLES)
    set(BUILD_EXAMPLES ON CACHE BOOL "Control example builds" FORCE)
else()
    set(BUILD_EXAMPLES OFF CACHE BOOL "Control example builds" FORCE)
endif()

if(LIBDMOTSERVICES_C_BUILD_TESTING)
    set(BUILD_TESTING ON CACHE BOOL "Control test builds" FORCE)
else()
    set(BUILD_TESTING OFF CACHE BOOL "Control test builds" FORCE)
endif()

add_subdirectory(linux)

add_custom_target(libdmotservices_c_test
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Executing libdmotservices C test suite")

if(TARGET libdmotservices_examples)
    add_custom_target(libdmotservices_c_examples
        DEPENDS libdmotservices_examples)
    add_custom_target(libdmotservices_c_example_demo
        DEPENDS libdmotservices_example_demo)
else()
    add_custom_target(libdmotservices_c_examples
        COMMAND ${CMAKE_COMMAND} -E echo "No libdmotservices C examples configured.")
    add_custom_target(libdmotservices_c_example_demo
        COMMAND ${CMAKE_COMMAND} -E echo "No libdmotservices C example demos available.")
endif()
