#################################################
## triangle-peg-game-react/CMakeLists.txt
## Usage:
##   cmake -S . -B build && cmake --build build
## Targets:
##   precompute     - generate tpg-spa/src/assets/states.json
##   build          - run npm install/build (depends on precompute)
##   example-demo   - stub (no examples yet)
##   clean-all      - remove build artifacts (tpg-spa/dist)
#################################################
cmake_minimum_required(VERSION 3.20)
project(triangle-peg-game-react NONE)

set(PRECOMPUTE_SCRIPT "${CMAKE_SOURCE_DIR}/precompute/precompute.py")
set(PRECOMPUTE_OUT "${CMAKE_SOURCE_DIR}/tpg-spa/src/assets/states.json")

add_custom_target(precompute ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Generating precomputed states..."
    COMMAND ${CMAKE_COMMAND} -E env python3 "${PRECOMPUTE_SCRIPT}" --export-json "${PRECOMPUTE_OUT}"
    BYPRODUCTS "${PRECOMPUTE_OUT}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    VERBATIM
)

add_custom_target(build ALL
    DEPENDS precompute
    COMMAND ${CMAKE_COMMAND} -E echo "Installing dependencies and building React app..."
    COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_SOURCE_DIR}/tpg-spa" npm ci
    COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_SOURCE_DIR}/tpg-spa" npm run build
    VERBATIM
)

add_custom_target(example-demo
    COMMAND ${CMAKE_COMMAND} -E echo "No examples to run for triangle-peg-game-react yet."
)

add_custom_target(tests
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tpg-spa"
    COMMAND npm test
    COMMENT "Running triangle-peg-game-react sanity checks"
)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E echo "Cleaning build artifacts..."
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${CMAKE_SOURCE_DIR}/tpg-spa/dist"
)
