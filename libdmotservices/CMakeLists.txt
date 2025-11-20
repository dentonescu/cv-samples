#
# Usage:
# cmake -S . -B build -G Ninja
# cmake --build build
# cmake --build build --target libdmotservices_test
# cmake --build build --target libdmotservices_example_demo

cmake_minimum_required(VERSION 3.16)

project(libdmotservices LANGUAGES C)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

file(READ "${CMAKE_CURRENT_LIST_DIR}/VERSION" LIBDMOTSERVICES_VERSION_RAW)
string(STRIP "${LIBDMOTSERVICES_VERSION_RAW}" LIBDMOTSERVICES_VERSION)

add_subdirectory(c)
add_subdirectory(js)
add_subdirectory(python)
add_subdirectory(java)

add_custom_target(libdmotservices_version
    COMMAND "${CMAKE_COMMAND}" -E echo "VERSION: ${LIBDMOTSERVICES_VERSION}"
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_LIST_DIR}/gen-version.py" > "${CMAKE_BINARY_DIR}/libdmotservices.version.log"
    COMMENT "Generating libdmotservices version data")

set(LIBDMOTSERVICES_DIST_DIR "${CMAKE_CURRENT_LIST_DIR}/dist")

# gather artifacts
file(GLOB_RECURSE LIBDMOTSERVICES_ARCHIVES
     "${CMAKE_CURRENT_LIST_DIR}/*.a")

file(GLOB_RECURSE LIBDMOTSERVICES_DIR_CANDIDATES LIST_DIRECTORIES true
     "${CMAKE_CURRENT_LIST_DIR}/*")

set(LIBDMOTSERVICES_BUILD_DIRS "")
set(LIBDMOTSERVICES_BIN_DIRS "")
foreach(path IN LISTS LIBDMOTSERVICES_DIR_CANDIDATES)
    if(path MATCHES "/node_modules/")
        continue()
    endif()
    if(IS_DIRECTORY "${path}")
        get_filename_component(_name "${path}" NAME)
        if(_name STREQUAL "build")
            list(APPEND LIBDMOTSERVICES_BUILD_DIRS "${path}")
        elseif(_name STREQUAL "bin")
            list(APPEND LIBDMOTSERVICES_BIN_DIRS "${path}")
        endif()
    endif()
endforeach()

add_custom_target(libdmotservices_dist
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${LIBDMOTSERVICES_DIST_DIR}")

foreach(_archive IN LISTS LIBDMOTSERVICES_ARCHIVES)
    if(_archive MATCHES "/node_modules/")
        continue()
    endif()
    add_custom_command(TARGET libdmotservices_dist
        COMMAND "${CMAKE_COMMAND}" -E copy "${_archive}" "${LIBDMOTSERVICES_DIST_DIR}"
        VERBATIM)
endforeach()

foreach(_builddir IN LISTS LIBDMOTSERVICES_BUILD_DIRS)
    add_custom_command(TARGET libdmotservices_dist
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${_builddir}" "${LIBDMOTSERVICES_DIST_DIR}"
        VERBATIM)
endforeach()

foreach(_bindir IN LISTS LIBDMOTSERVICES_BIN_DIRS)
    add_custom_command(TARGET libdmotservices_dist
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${_bindir}" "${LIBDMOTSERVICES_DIST_DIR}"
        VERBATIM)
endforeach()

add_dependencies(libdmotservices_dist
    libdmotservices_version libdmotservices libdmotservices_js libdmotservices_py libdmotservices_java_dist)

add_custom_target(libdmotservices_all
    DEPENDS libdmotservices_version libdmotservices libdmotservices_js libdmotservices_py libdmotservices_java_all)

add_custom_target(libdmotservices_test
    DEPENDS libdmotservices_c_test libdmotservices_js_test libdmotservices_py_test libdmotservices_java_test)

if(TARGET libdmotservices_c_examples)
    add_custom_target(libdmotservices_examples
        DEPENDS libdmotservices_c_examples)
else()
    add_custom_target(libdmotservices_examples
        COMMAND "${CMAKE_COMMAND}" -E echo "No C examples are configured.")
endif()

if(TARGET libdmotservices_c_example_demo
   OR TARGET libdmotservices_js_example_demo
   OR TARGET libdmotservices_py_example_demo)
    add_custom_target(libdmotservices_example_demo)
    if(TARGET libdmotservices_c_example_demo)
        add_dependencies(libdmotservices_example_demo libdmotservices_c_example_demo)
    endif()
    if(TARGET libdmotservices_js_example_demo)
        add_dependencies(libdmotservices_example_demo libdmotservices_js_example_demo)
    endif()
    if(TARGET libdmotservices_py_example_demo)
        add_dependencies(libdmotservices_example_demo libdmotservices_py_example_demo)
    endif()
else()
    add_custom_target(libdmotservices_example_demo
        COMMAND "${CMAKE_COMMAND}" -E echo "No libdmotservices example demos defined.")
endif()
