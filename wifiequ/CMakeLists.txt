#
# Usage:
# cmake -S . -B build -G Ninja
# cmake --build build
# cmake --build build --target wifiequ_dist

cmake_minimum_required(VERSION 3.16)

project(wifiequ LANGUAGES C)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
find_program(WIFIEQU_NPM_EXECUTABLE npm)

file(READ "${CMAKE_CURRENT_LIST_DIR}/VERSION" WIFIEQU_VERSION_RAW)
string(STRIP "${WIFIEQU_VERSION_RAW}" WIFIEQU_VERSION)

add_subdirectory(linux)

add_custom_target(wifiequ_version
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_LIST_DIR}/gen-version.py"
    COMMENT "Generating WiFiEqu version metadata")

set(WIFIEQU_DIST_DIR "${CMAKE_CURRENT_LIST_DIR}/dist")
set(WIFIEQU_LINUX_ARTIFACTS "${CMAKE_CURRENT_BINARY_DIR}/linux/artifacts")
set(WIFIEQU_FRONTEND_DIR "${CMAKE_CURRENT_LIST_DIR}/web-angular/wifiequ-frontend")

add_custom_target(wifiequ_dist
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${WIFIEQU_DIST_DIR}"
    DEPENDS wifiequ_version wifiequd
    COMMENT "Populating WiFiEqu distribution directory")

add_custom_command(TARGET wifiequ_dist
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${WIFIEQU_LINUX_ARTIFACTS}/bin" "${WIFIEQU_DIST_DIR}/bin"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${WIFIEQU_LINUX_ARTIFACTS}/lib" "${WIFIEQU_DIST_DIR}/lib"
    VERBATIM)

find_program(NPX_EXECUTABLE npx)
set(WIFIEQU_OPENAPI "${CMAKE_CURRENT_LIST_DIR}/api/openapi.yaml")
set(WIFIEQU_API_HTML "${CMAKE_CURRENT_LIST_DIR}/docs/api/index.html")

if(NPX_EXECUTABLE)
    add_custom_target(wifiequ_docs_openapi
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_CURRENT_LIST_DIR}/docs/api"
        COMMAND "${NPX_EXECUTABLE}" @redocly/cli build-docs "${WIFIEQU_OPENAPI}" --output "${WIFIEQU_API_HTML}"
        COMMENT "Generating OpenAPI HTML documentation")
else()
    add_custom_target(wifiequ_docs_openapi
        COMMAND "${CMAKE_COMMAND}" -E echo "npx not available; skipping OpenAPI docs.")
endif()

if(WIFIEQU_NPM_EXECUTABLE)
    add_custom_target(wifiequ_frontend_build
        COMMAND "${CMAKE_COMMAND}" -E echo "Building WiFiEqu frontend with npm"
        COMMAND "${CMAKE_COMMAND}" -E chdir "${WIFIEQU_FRONTEND_DIR}" ${WIFIEQU_NPM_EXECUTABLE} ci
        COMMAND "${CMAKE_COMMAND}" -E chdir "${WIFIEQU_FRONTEND_DIR}" ${WIFIEQU_NPM_EXECUTABLE} run build
        COMMENT "Angular build for WiFiEqu frontend")

    add_custom_target(wifiequ_frontend_test
        COMMAND "${CMAKE_COMMAND}" -E echo "Running WiFiEqu frontend tests"
        COMMAND "${CMAKE_COMMAND}" -E chdir "${WIFIEQU_FRONTEND_DIR}" ${WIFIEQU_NPM_EXECUTABLE} test -- --watch=false
        COMMENT "Angular tests for WiFiEqu frontend")
else()
    add_custom_target(wifiequ_frontend_build
        COMMAND "${CMAKE_COMMAND}" -E echo "npm not available; skipping WiFiEqu frontend build."
        COMMENT "Skipping WiFiEqu frontend build (npm not found)"
    )
    add_custom_target(wifiequ_frontend_test
        COMMAND "${CMAKE_COMMAND}" -E echo "npm not available; skipping WiFiEqu frontend tests."
        COMMENT "Skipping WiFiEqu frontend tests (npm not found)"
    )
endif()

add_custom_target(wifiequ_tests
    DEPENDS wifiequ_frontend_test
    COMMAND "${CMAKE_CTEST_COMMAND}" --output-on-failure
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Executing WiFiEqu test suite"
)

if(TARGET wifiequ_example_demo)
    add_custom_target(wifiequ_examples DEPENDS wifiequ_example_demo)
else()
    add_custom_target(wifiequ_examples
        COMMAND "${CMAKE_COMMAND}" -E echo "No WiFiEqu examples defined.")
endif()

add_custom_target(wifiequ_all
    DEPENDS wifiequ_version wifiequd wifiequ_frontend_build)
