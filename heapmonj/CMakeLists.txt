#
# Usage:
#     cmake -S . -B build -G Ninja
#     cmake --build build --target heapmonj_all
#     cmake --build build --target heapmonj_test
#     cmake --build build --target heapmonj_docs_openapi
#     cmake --build build --target heapmonj_example_demo


cmake_minimum_required(VERSION 3.16)

project(heapmonj_docs LANGUAGES NONE)

find_program(HEAPMONJ_NPX_EXECUTABLE npx)
find_program(HEAPMONJ_NPM_EXECUTABLE npm)
find_program(HEAPMONJ_SH_EXECUTABLE sh)

set(HEAPMONJ_OPENAPI "${CMAKE_CURRENT_LIST_DIR}/api/openapi.yaml")
set(HEAPMONJ_DOC_API_DIR "${CMAKE_CURRENT_LIST_DIR}/docs/api")
set(HEAPMONJ_DOC_API_HTML "${HEAPMONJ_DOC_API_DIR}/index.html")
set(HEAPMONJ_BACKEND_DIR "${CMAKE_CURRENT_LIST_DIR}/backend/heapmonj")
set(HEAPMONJ_FRONTEND_DIR "${CMAKE_CURRENT_LIST_DIR}/frontend/heapmonj-frontend")

add_custom_target(heapmonj_docs ALL
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${HEAPMONJ_DOC_API_DIR}"
    COMMENT "Generating heapmonj OpenAPI HTML docs")

if(HEAPMONJ_NPX_EXECUTABLE)
    add_custom_target(heapmonj_docs_openapi
        DEPENDS heapmonj_docs
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E echo "Building heapmonj OpenAPI HTML docs with redocly"
        COMMAND ${HEAPMONJ_NPX_EXECUTABLE} @redocly/cli build-docs "${HEAPMONJ_OPENAPI}" --output "${HEAPMONJ_DOC_API_HTML}"
        COMMENT "Building heapmonj OpenAPI HTML docs with redocly")
else()
    add_custom_target(heapmonj_docs_openapi
        DEPENDS heapmonj_docs
        COMMAND ${CMAKE_COMMAND} -E echo "Skipping heapmonj OpenAPI HTML docs (npx not found; install @redocly/cli to enable)"
        COMMENT "Skipping heapmonj OpenAPI HTML docs (npx not found)")
endif()

add_custom_target(heapmonj_backend_build
    COMMAND ${CMAKE_COMMAND} -E echo "Building heapmonj backend with Gradle"
    COMMAND ${CMAKE_COMMAND} -E chdir "${HEAPMONJ_BACKEND_DIR}" ./gradlew clean build
    COMMENT "Gradle build for heapmonj backend")

add_custom_target(heapmonj_frontend_build
    COMMAND ${CMAKE_COMMAND} -E echo "Building heapmonj frontend with npm"
    COMMAND ${CMAKE_COMMAND} -E chdir "${HEAPMONJ_FRONTEND_DIR}" ${HEAPMONJ_NPM_EXECUTABLE} ci
    COMMAND ${CMAKE_COMMAND} -E chdir "${HEAPMONJ_FRONTEND_DIR}" ${HEAPMONJ_NPM_EXECUTABLE} run build
    COMMENT "Angular build for heapmonj frontend")

add_custom_target(heapmonj_all
    DEPENDS heapmonj_backend_build heapmonj_frontend_build heapmonj_docs_openapi
    COMMENT "Build backend, frontend, and OpenAPI docs")

add_custom_target(heapmonj_clean
    COMMAND ${CMAKE_COMMAND} -E echo "Cleaning backend and frontend artifacts"
    COMMAND ${CMAKE_COMMAND} -E chdir "${HEAPMONJ_BACKEND_DIR}" ./gradlew clean
    COMMAND ${CMAKE_COMMAND} -E chdir "${HEAPMONJ_FRONTEND_DIR}" ${CMAKE_COMMAND} -E rm -rf dist out-tsc
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${HEAPMONJ_BACKEND_DIR}/data"
    COMMENT "Clean backend and frontend build outputs")

add_custom_target(heapmonj_test
    COMMAND ${CMAKE_COMMAND} -E echo "Running backend tests"
    COMMAND ${CMAKE_COMMAND} -E chdir "${HEAPMONJ_BACKEND_DIR}" ./gradlew test --rerun-tasks
    COMMAND ${CMAKE_COMMAND} -E echo "Running frontend tests"
    COMMAND ${CMAKE_COMMAND} -E chdir "${HEAPMONJ_FRONTEND_DIR}" ${HEAPMONJ_NPM_EXECUTABLE} test -- --watch=false
    COMMENT "Run backend and frontend tests")

add_custom_target(heapmonj_example_demo
    COMMAND ${CMAKE_COMMAND} -E echo "No runnable examples/demos for heapmonj yet (API + UI only)."
    COMMENT "Placeholder for future examples/demos")
