#
# Usage:
# cmake -S . -B build -G Ninja
# cmake --build build
# cmake --build build --target libdmotservices_py_test
# cmake --build build --target libdmotservices_py_vendor

cmake_minimum_required(VERSION 3.16)

project(libdmotservices_python LANGUAGES NONE)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

file(READ "${CMAKE_CURRENT_LIST_DIR}/../VERSION" LIBDMOTSERVICES_PY_VERSION_RAW)
string(STRIP "${LIBDMOTSERVICES_PY_VERSION_RAW}" LIBDMOTSERVICES_PY_VERSION)

# keep pyproject.toml version in sync with VERSION
file(READ "${CMAKE_CURRENT_LIST_DIR}/pyproject.toml" _pyproj_data)
string(REGEX REPLACE "^[ \t]*version[ \t]*=[ \t]*\".*\""
                     "version = \"${LIBDMOTSERVICES_PY_VERSION}\""
                     _pyproj_data "${_pyproj_data}")
file(WRITE "${CMAKE_CURRENT_LIST_DIR}/pyproject.toml" "${_pyproj_data}")

set(PY_PACKAGE_DIR "${CMAKE_CURRENT_LIST_DIR}/libdmotservices")
set(PY_VENDOR_ROOT "${CMAKE_CURRENT_LIST_DIR}/vendor")
set(PY_VENDOR_PACKAGE "${PY_VENDOR_ROOT}/libdmotservices")
set(PY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/build")
set(PY_DIST_DIR "${CMAKE_CURRENT_BINARY_DIR}/dist")
set(PY_DIST_NAME "libdmotservices-py-${LIBDMOTSERVICES_PY_VERSION}")
set(PY_DIST_STAGE "${PY_DIST_DIR}/${PY_DIST_NAME}")

add_custom_target(libdmotservices_py_version
    COMMAND "${CMAKE_COMMAND}" -E copy
            "${CMAKE_CURRENT_LIST_DIR}/../VERSION"
            "${PY_PACKAGE_DIR}/.version"
    COMMENT "Synchronizing Python package version metadata"
)

add_custom_target(libdmotservices_py_build
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${PY_BUILD_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${PY_BUILD_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E env
            PYTHONPYCACHEPREFIX="${PY_BUILD_DIR}"
            "${Python3_EXECUTABLE}" -m compileall -q libdmotservices
    DEPENDS libdmotservices_py_version
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    COMMENT "Compiling libdmotservices Python bytecode"
)

add_custom_target(libdmotservices_py ALL
    DEPENDS libdmotservices_py_build)

add_custom_target(libdmotservices_py_vendor
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${PY_VENDOR_PACKAGE}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${PY_VENDOR_ROOT}"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${PY_PACKAGE_DIR}" "${PY_VENDOR_PACKAGE}"
    DEPENDS libdmotservices_py_build
    COMMENT "Vendoring libdmotservices Python package"
)

add_custom_target(libdmotservices_py_test
    COMMAND "${Python3_EXECUTABLE}" -m pytest -svv -o log-cli=true --log-cli-level=DEBUG
    DEPENDS libdmotservices_py_vendor
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    COMMENT "Running libdmotservices Python tests"
)

add_custom_target(libdmotservices_py_dist
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${PY_DIST_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${PY_DIST_STAGE}"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/README.md" "${PY_DIST_STAGE}/README.md"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/../VERSION" "${PY_DIST_STAGE}/VERSION"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${PY_PACKAGE_DIR}" "${PY_DIST_STAGE}/libdmotservices"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${PY_BUILD_DIR}" "${PY_DIST_STAGE}/build"
    COMMAND "${CMAKE_COMMAND}" -E tar "cfvz" "${PY_DIST_DIR}/${PY_DIST_NAME}.tar.gz" --format=gnutar -C "${PY_DIST_DIR}" "${PY_DIST_NAME}"
    DEPENDS libdmotservices_py_build
    COMMENT "Packaging libdmotservices Python distribution"
)

add_custom_target(libdmotservices_py_example_demo
    COMMAND "${CMAKE_COMMAND}" -E echo "No Python examples to execute."
)
