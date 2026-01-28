#################################################
## pwa-llm-poc/CMakeLists.txt
## Usage:
##   cmake -S . -B build && cmake --build build
## Targets:
##   pwa-llm-poc-build          - run npm install/build
##   pwa-llm-poc-dist           - build and describe dist output
##   pwa-llm-poc-examples       - stub (no examples yet)
##   pwa-llm-poc-example-demo   - stub (no examples yet)
##   pwa-llm-poc-tests          - stub (no tests yet)
##   pwa-llm-poc-test           - alias for tests
##   pwa-llm-poc-clean          - remove build artifacts (frontend/llm-spa/dist)
##   pwa-llm-poc-run            - launch frontend + backend
##   pwa-llm-poc-docs           - generate OpenAPI HTML docs
#################################################
cmake_minimum_required(VERSION 3.20)
project(pwa-llm-poc NONE)

add_custom_target(pwa-llm-poc-build ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Installing dependencies and building React app..."
    COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_SOURCE_DIR}/frontend/llm-spa" npm ci
    COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_SOURCE_DIR}/frontend/llm-spa" npm run build
    VERBATIM
)

add_custom_target(pwa-llm-poc-dist
    COMMAND ${CMAKE_COMMAND} -E echo "Dist artifacts are in frontend/llm-spa/dist"
    DEPENDS pwa-llm-poc-build
)

add_custom_target(pwa-llm-poc-examples
    COMMAND ${CMAKE_COMMAND} -E echo "No compilation necessary for examples."
)

add_custom_target(pwa-llm-poc-example-demo
    COMMAND ${CMAKE_COMMAND} -E echo "No examples to run for pwa-llm-poc yet."
)

add_custom_target(pwa-llm-poc-tests
    COMMAND ${CMAKE_COMMAND} -E echo "No compilation necessary for tests."
)

add_custom_target(pwa-llm-poc-test
    COMMAND ${CMAKE_COMMAND} -E echo "No unit tests for pwa-llm-poc yet."
    DEPENDS pwa-llm-poc-tests
)

add_custom_target(pwa-llm-poc-clean
    COMMAND ${CMAKE_COMMAND} -E echo "Cleaning build artifacts..."
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${CMAKE_SOURCE_DIR}/frontend/llm-spa/dist"
)

add_custom_target(pwa-llm-poc-run
    COMMAND ${CMAKE_COMMAND} -E echo "Launching the frontend and backend..."
    COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_SOURCE_DIR}" ./run-pwa-llm.sh
)

add_custom_target(pwa-llm-poc-docs
    COMMAND ${CMAKE_COMMAND} -E echo "Generating OpenAPI HTML docs..."
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/docs/api"
    COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_SOURCE_DIR}" npx --yes @redocly/cli build-docs api/openapi.yaml -o docs/api/index.html
)
