#
# Usage:
# cmake -S . -B build -G Ninja
# cmake --build build
# ctest --test-dir build
# cmake --build build --target libdmotservices_c_linux_example_demo

cmake_minimum_required(VERSION 3.16)

file(READ "${CMAKE_CURRENT_LIST_DIR}/../../VERSION" LIBDMOTSERVICES_VERSION_RAW)
string(STRIP "${LIBDMOTSERVICES_VERSION_RAW}" LIBDMOTSERVICES_VERSION)

project(libdmotservices_c_linux
        VERSION "${LIBDMOTSERVICES_VERSION}"
        LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

if(UNIX AND NOT APPLE)
    set(MATH_LIBRARY m)
else()
    set(MATH_LIBRARY "")
endif()

set(LIB_TARGET libdmotservices)

# recursively put src/*.c into LIBDMOTSERVICES_SOURCES; re-run configure if matches change
file(GLOB_RECURSE LIBDMOTSERVICES_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_LIST_DIR}/src/*.c")

add_library(${LIB_TARGET} STATIC ${LIBDMOTSERVICES_SOURCES})
target_include_directories(${LIB_TARGET}
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/include"
)

if(MSVC)
    target_compile_options(${LIB_TARGET} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${LIB_TARGET} PRIVATE -O2 -Wall -Wextra)
endif()

set_target_properties(${LIB_TARGET} PROPERTIES
    OUTPUT_NAME "${LIB_TARGET}_c-${PROJECT_VERSION}"
)

include(CTest)

if(BUILD_TESTING)
    enable_testing()
    find_library(CMOCKA_LIBRARY NAMES cmocka)
    if(NOT CMOCKA_LIBRARY)
        message(FATAL_ERROR "cmocka library is required to build tests.")
    endif()

    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
         "${CMAKE_CURRENT_LIST_DIR}/tests/*.c")

    foreach(test_src IN LISTS TEST_SOURCES)
        get_filename_component(test_name "${test_src}" NAME_WE)
        set(test_target "libdmotservices_${test_name}")
        add_executable("${test_target}" "${test_src}")
        target_link_libraries("${test_target}"
            PRIVATE
                ${LIB_TARGET}
                "${CMOCKA_LIBRARY}"
                "${MATH_LIBRARY}"
        )
        add_test(NAME "${test_target}" COMMAND "${test_target}")
    endforeach()
endif()

# "cmake -S . -B build -DBUILD_EXAMPLES=off" to skip building the example executables
option(BUILD_EXAMPLES "Build the sample example executables." ON)

set(libdmotservices_c_linux_example_TARGETS "")
if(BUILD_EXAMPLES)
    file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS
         "${CMAKE_CURRENT_LIST_DIR}/examples/*.c")
    foreach(ex_src IN LISTS EXAMPLE_SOURCES)
        get_filename_component(ex_name "${ex_src}" NAME_WE)
        add_executable("${ex_name}" "${ex_src}")
        target_link_libraries("${ex_name}"
            PRIVATE
                ${LIB_TARGET}
                "${MATH_LIBRARY}"
        )
        list(APPEND libdmotservices_c_linux_example_TARGETS "${ex_name}")
    endforeach()
endif()

if(libdmotservices_c_linux_example_TARGETS)
    add_custom_target(libdmotservices_c_linux_examples
        DEPENDS ${libdmotservices_c_linux_example_TARGETS})

    add_custom_target(libdmotservices_c_linux_example_demo
        DEPENDS libdmotservices_c_linux_examples
        COMMENT "Executing libdmotservices C examples")

    foreach(ex IN LISTS libdmotservices_c_linux_example_TARGETS)
        add_custom_command(TARGET libdmotservices_c_linux_example_demo
            COMMAND ${CMAKE_COMMAND} -E echo "Running: ${ex}"
            COMMAND "$<TARGET_FILE:${ex}>"
        )
    endforeach()

else()
    add_custom_target(libdmotservices_c_linux_examples
        COMMAND "${CMAKE_COMMAND}" -E echo "No C examples to build.")
    add_custom_target(libdmotservices_c_linux_example_demo
        COMMAND "${CMAKE_COMMAND}" -E echo "No C examples to run.")
endif()
