#
# Usage:
# cmake -S . -B build -G Ninja
# cmake --build build
# cmake --build build --target slideshow_dist
# cmake --build build --target slideshow_test

cmake_minimum_required(VERSION 3.16)

project(slideshow_server LANGUAGES NONE)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

file(READ "${CMAKE_CURRENT_LIST_DIR}/VERSION" SLIDESHOW_VERSION_RAW)
string(STRIP "${SLIDESHOW_VERSION_RAW}" SLIDESHOW_VERSION)

set(SLIDESHOW_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/pybuild")
set(SLIDESHOW_DIST_DIR "${CMAKE_CURRENT_BINARY_DIR}/dist")
set(SLIDESHOW_DIST_NAME "slideshow-server-${SLIDESHOW_VERSION}")
set(SLIDESHOW_DIST_STAGE "${SLIDESHOW_DIST_DIR}/${SLIDESHOW_DIST_NAME}")
set(SLIDESHOW_LIBDMOT_PY_ROOT "${CMAKE_CURRENT_LIST_DIR}/../libdmotservices/python")

if(TARGET libdmotservices_py_vendor)
    add_custom_target(slideshow_vendor
        DEPENDS libdmotservices_py_vendor)
else()
    set(SLIDESHOW_LIBDMOT_BUILD "${CMAKE_CURRENT_BINARY_DIR}/_libdmot_py")
    add_custom_target(slideshow_vendor
        COMMAND "${CMAKE_COMMAND}" -S "${SLIDESHOW_LIBDMOT_PY_ROOT}" -B "${SLIDESHOW_LIBDMOT_BUILD}"
        COMMAND "${CMAKE_COMMAND}" --build "${SLIDESHOW_LIBDMOT_BUILD}" --target libdmotservices_py_vendor
        COMMENT "Vendoring libdmotservices for slideshow-server")
endif()

add_custom_target(slideshow_version
    COMMAND "${CMAKE_COMMAND}" -E copy
            "${CMAKE_CURRENT_LIST_DIR}/VERSION"
            "${CMAKE_CURRENT_LIST_DIR}/slideshow_server/.version"
    COMMENT "Writing slideshow-server version marker")

set(SLIDESHOW_PYTHON_ENV "PYTHONPATH=${SLIDESHOW_LIBDMOT_PY_ROOT}:${SLIDESHOW_LIBDMOT_PY_ROOT}/vendor:\$PYTHONPATH")

file(MAKE_DIRECTORY "${SLIDESHOW_BUILD_DIR}")
add_custom_target(slideshow_pre_build
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${SLIDESHOW_BUILD_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${SLIDESHOW_BUILD_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${CMAKE_CURRENT_LIST_DIR}/slideshow_server"
            "${SLIDESHOW_BUILD_DIR}/slideshow_server"
    COMMAND "${CMAKE_COMMAND}" -E copy
            "${CMAKE_CURRENT_LIST_DIR}/slideshow-server.py"
            "${SLIDESHOW_BUILD_DIR}/slideshow-server.py"
    DEPENDS slideshow_vendor slideshow_version
    WORKING_DIRECTORY "${SLIDESHOW_BUILD_DIR}"
    COMMENT "Populating the build directory"
)
add_custom_target(slideshow_build
    COMMAND "${CMAKE_COMMAND}" -E env
            ${SLIDESHOW_PYTHON_ENV}
            "${Python3_EXECUTABLE}" -m compileall -q pybuild/slideshow_server pybuild/slideshow-server.py
    WORKING_DIRECTORY "${SLIDESHOW_BUILD_DIR}"
    DEPENDS slideshow_pre_build
    COMMENT "Compiling slideshow-server bytecode")

add_custom_target(slideshow_test
    COMMAND "${CMAKE_COMMAND}" -E env
            ${SLIDESHOW_PYTHON_ENV}
            "${Python3_EXECUTABLE}" -m pytest -svv -o log-cli=true --log-cli-level=DEBUG
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    DEPENDS slideshow_vendor slideshow_version
    COMMENT "Running slideshow-server unit tests")

file(MAKE_DIRECTORY "${SLIDESHOW_DIST_DIR}")
add_custom_target(slideshow_pre_dist
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${SLIDESHOW_DIST_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${SLIDESHOW_DIST_STAGE}/vendor"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${SLIDESHOW_LIBDMOT_PY_ROOT}/vendor/libdmotservices" "${SLIDESHOW_DIST_STAGE}/vendor/libdmotservices"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/README.md" "${SLIDESHOW_DIST_STAGE}/README.md"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/run-demo.sh" "${SLIDESHOW_DIST_STAGE}/run-demo.sh"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/slideshow-server.py" "${SLIDESHOW_DIST_STAGE}/slideshow-server.py"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/slideshow-server.css" "${SLIDESHOW_DIST_STAGE}/slideshow-server.css"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/slideshow-server.html" "${SLIDESHOW_DIST_STAGE}/slideshow-server.html"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/slideshow-server.js" "${SLIDESHOW_DIST_STAGE}/slideshow-server.js"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/VERSION" "${SLIDESHOW_DIST_STAGE}/VERSION"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/slideshow_server" "${SLIDESHOW_DIST_STAGE}/slideshow_server"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${SLIDESHOW_BUILD_DIR}" "${SLIDESHOW_DIST_STAGE}/build"
    WORKING_DIRECTORY "${SLIDESHOW_DIST_DIR}"
    DEPENDS slideshow_build slideshow_vendor
    COMMENT "Populating the dist directory"
)

add_custom_target(slideshow_dist
    COMMAND "${CMAKE_COMMAND}" -E tar "cfvz" "${SLIDESHOW_DIST_NAME}.tar.gz" "${SLIDESHOW_DIST_NAME}"
    DEPENDS slideshow_pre_dist
    WORKING_DIRECTORY "${SLIDESHOW_DIST_DIR}"
    COMMENT "Packaging slideshow-server ${SLIDESHOW_VERSION}"
)

add_custom_target(slideshow_all
    DEPENDS slideshow_build)

add_custom_target(slideshow_example_demo
    COMMAND "${CMAKE_COMMAND}" -E echo "No slideshow-server examples are defined.")
