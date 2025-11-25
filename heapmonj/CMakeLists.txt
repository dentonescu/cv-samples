cmake_minimum_required(VERSION 3.16)

project(heapmonj_docs LANGUAGES NONE)

find_program(HEAPMONJ_NPX_EXECUTABLE npx)

set(HEAPMONJ_OPENAPI "${CMAKE_CURRENT_LIST_DIR}/api/openapi.yaml")
set(HEAPMONJ_DOC_API_DIR "${CMAKE_CURRENT_LIST_DIR}/docs/api")
set(HEAPMONJ_DOC_API_HTML "${HEAPMONJ_DOC_API_DIR}/index.html")

add_custom_target(heapmonj_docs ALL
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${HEAPMONJ_DOC_API_DIR}"
    COMMENT "Generating heapmonj OpenAPI HTML docs")

add_custom_target(heapmonj_docs_openapi
    DEPENDS heapmonj_docs
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E env HEAPMONJ_OPENAPI="${HEAPMONJ_OPENAPI}" HEAPMONJ_DOC_API_HTML="${HEAPMONJ_DOC_API_HTML}"
            ${CMAKE_COMMAND} -E sh -c "if ! ${HEAPMONJ_NPX_EXECUTABLE} --no-install @redocly/cli --version >/dev/null 2>&1; then echo \"@redocly/cli not found; install locally (npm install --save-dev @redocly/cli)\"; fi"
    COMMAND ${CMAKE_COMMAND} -E sh -c "${HEAPMONJ_NPX_EXECUTABLE} @redocly/cli build-docs \"${HEAPMONJ_OPENAPI}\" --output \"${HEAPMONJ_DOC_API_HTML}\""
    COMMENT "Building heapmonj OpenAPI HTML docs with redocly")
