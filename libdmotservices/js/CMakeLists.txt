#
# Usage:
# cmake -S . -B build -G Ninja
# cmake --build build
# cmake --build build --target libdmotservices_js_test
# cmake --build build --target libdmotservices_js_vendor

cmake_minimum_required(VERSION 3.16)

project(libdmotservices_js LANGUAGES NONE)

set(JS_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(JS_DIST_DIR   "${CMAKE_CURRENT_LIST_DIR}/dist")
set(JS_DIST_FILE  "${JS_DIST_DIR}/index.js")
set(JS_VENDOR_DIR "${CMAKE_CURRENT_LIST_DIR}/vendor/libdmotservices")
set(JS_VENDOR_FILE "${JS_VENDOR_DIR}/index.js")

set(JS_SOURCES
    "${JS_SOURCE_DIR}/00-banner.js"
    "${JS_SOURCE_DIR}/logger.js"
    "${JS_SOURCE_DIR}/formdata.js"
    "${JS_SOURCE_DIR}/exports.js"
)

message(STATUS "Bundling libdmotservices JavaScript helpers")
file(WRITE "${JS_VENDOR_FILE}" "")
foreach(f IN LISTS JS_SOURCES)
  file(READ "${f}" _content)
  file(APPEND "${JS_VENDOR_FILE}" "${_content}")
endforeach()

add_custom_target(libdmotservices_js ALL
    DEPENDS "${JS_DIST_FILE}"
    COMMENT "Generating libdmotservices JavaScript bundle")

add_custom_target(libdmotservices_js_vendor
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${JS_VENDOR_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy "${JS_DIST_FILE}" "${JS_VENDOR_FILE}"
    DEPENDS libdmotservices_js
    COMMENT "Copying bundle to vendor/libdmotservices")

find_program(NPM_EXECUTABLE npm)

if(NPM_EXECUTABLE)
    add_custom_target(libdmotservices_js_lint
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
        COMMAND "${NPM_EXECUTABLE}" install
        COMMAND "${NPM_EXECUTABLE}" run lint
        COMMENT "Linting libdmotservices JS sources"
    )

    add_custom_target(libdmotservices_js_test
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
        COMMAND "${NPM_EXECUTABLE}" run test
        DEPENDS libdmotservices_js_lint libdmotservices_js
        COMMENT "Running libdmotservices JS unit tests"
    )
else()
    add_custom_target(libdmotservices_js_lint
        COMMAND "${CMAKE_COMMAND}" -E echo "npm not available; skipping JS lint."
    )
    add_custom_target(libdmotservices_js_test
        COMMAND "${CMAKE_COMMAND}" -E echo "npm not available; skipping JS tests."
        DEPENDS libdmotservices_js
    )
endif()

add_custom_target(libdmotservices_js_example_demo
    COMMAND "${CMAKE_COMMAND}" -E echo "No JS examples to execute."
)
