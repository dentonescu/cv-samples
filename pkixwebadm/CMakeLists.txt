#
# Usage:
# cmake -S . -B build -G Ninja
# cmake --build build
# cmake --build build --target pkixwebadm_dist
# cmake --build build --target pkixwebadm_test

cmake_minimum_required(VERSION 3.16)

project(pkixwebadm LANGUAGES NONE)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

file(READ "${CMAKE_CURRENT_LIST_DIR}/VERSION" PKIXWA_VERSION_RAW)
string(STRIP "${PKIXWA_VERSION_RAW}" PKIXWA_VERSION)

set(PKIXWA_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/build")
set(PKIXWA_DIST_DIR "${CMAKE_CURRENT_BINARY_DIR}/dist")
set(PKIXWA_DIST_NAME "pkixwebadm-${PKIXWA_VERSION}")
set(PKIXWA_DIST_STAGE "${PKIXWA_DIST_DIR}/${PKIXWA_DIST_NAME}")
set(PKIXWA_LIBDMOT_PY_ROOT "${CMAKE_CURRENT_LIST_DIR}/../libdmotservices/python")
set(PKIXWA_LIBDMOT_JS_ROOT "${CMAKE_CURRENT_LIST_DIR}/../libdmotservices/js")
set(PKIXWA_VENDOR_JS_DIR "${CMAKE_CURRENT_LIST_DIR}/pkixwebadm/web/static/vendor/libdmotservices")
set(PKIXWA_VENDOR_JS_FILE "${PKIXWA_VENDOR_JS_DIR}/index.js")
set(PKIXWA_LIBDMOT_JS_VENDOR_FILE "${PKIXWA_LIBDMOT_JS_ROOT}/vendor/libdmotservices/index.js")

if(TARGET libdmotservices_py_vendor)
    add_custom_target(pkixwebadm_vendor_py DEPENDS libdmotservices_py_vendor)
else()
    set(PKIXWA_LIBDMOT_PY_BUILD "${CMAKE_CURRENT_BINARY_DIR}/_libdmot_py")
    add_custom_target(pkixwebadm_vendor_py
        COMMAND "${CMAKE_COMMAND}" -S "${PKIXWA_LIBDMOT_PY_ROOT}" -B "${PKIXWA_LIBDMOT_PY_BUILD}"
        COMMAND "${CMAKE_COMMAND}" --build "${PKIXWA_LIBDMOT_PY_BUILD}" --target libdmotservices_py_vendor
        COMMENT "Vendoring libdmotservices Python package for pkixwebadm")
endif()

if(TARGET libdmotservices_js_vendor)
    add_custom_target(pkixwebadm_vendor_js DEPENDS libdmotservices_js_vendor)
else()
    set(PKIXWA_LIBDMOT_JS_BUILD "${CMAKE_CURRENT_BINARY_DIR}/_libdmot_js")
    add_custom_target(pkixwebadm_vendor_js
        COMMAND "${CMAKE_COMMAND}" -S "${PKIXWA_LIBDMOT_JS_ROOT}" -B "${PKIXWA_LIBDMOT_JS_BUILD}"
        COMMAND "${CMAKE_COMMAND}" --build "${PKIXWA_LIBDMOT_JS_BUILD}" --target libdmotservices_js_vendor
        COMMENT "Vendoring libdmotservices JavaScript bundle for pkixwebadm")
endif()

add_custom_target(pkixwebadm_vendor
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${PKIXWA_VENDOR_JS_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy "${PKIXWA_LIBDMOT_JS_VENDOR_FILE}" "${PKIXWA_VENDOR_JS_FILE}"
    DEPENDS pkixwebadm_vendor_py pkixwebadm_vendor_js
    COMMENT "Vendoring libdmotservices for pkixwebadm")

add_custom_target(pkixwebadm_version
    COMMAND "${CMAKE_COMMAND}" -E copy
            "${CMAKE_CURRENT_LIST_DIR}/VERSION"
            "${CMAKE_CURRENT_LIST_DIR}/pkixwebadm/.version"
    COMMENT "Writing pkixwebadm version marker")

set(PKIXWA_PYTHON_ENV "PYTHONPATH=${PKIXWA_LIBDMOT_PY_ROOT}:${PKIXWA_LIBDMOT_PY_ROOT}/vendor:\$PYTHONPATH")

add_custom_target(pkixwebadm_build
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${PKIXWA_BUILD_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${PKIXWA_BUILD_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E env
            ${PKIXWA_PYTHON_ENV}
            PYTHONPYCACHEPREFIX="${PKIXWA_BUILD_DIR}"
            "${Python3_EXECUTABLE}" -m compileall -q pkixwebadm pkixwebadm.py
    DEPENDS pkixwebadm_vendor pkixwebadm_version
    COMMENT "Compiling pkixwebadm bytecode")

find_program(NPM_EXECUTABLE npm)

if(NPM_EXECUTABLE)
    add_custom_target(pkixwebadm_lint_js
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
        COMMAND "${NPM_EXECUTABLE}" install
        COMMAND "${NPM_EXECUTABLE}" run lint:js
        COMMENT "Linting pkixwebadm JavaScript sources")
else()
    add_custom_target(pkixwebadm_lint_js
        COMMAND "${CMAKE_COMMAND}" -E echo "npm not found; skipping JS lint.")
endif()

add_custom_target(pkixwebadm_test
    COMMAND "${CMAKE_COMMAND}" -E env
            ${PKIXWA_PYTHON_ENV}
            "${Python3_EXECUTABLE}" -m pytest -svv -o log-cli=true --log-cli-level=DEBUG
    DEPENDS pkixwebadm_vendor pkixwebadm_lint_js pkixwebadm_version
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
    COMMENT "Running pkixwebadm unit tests")

add_custom_target(pkixwebadm_dist
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${PKIXWA_DIST_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${PKIXWA_DIST_STAGE}/vendor"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${PKIXWA_LIBDMOT_PY_ROOT}/vendor/libdmotservices" "${PKIXWA_DIST_STAGE}/vendor/libdmotservices"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/README.md" "${PKIXWA_DIST_STAGE}/README.md"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/pkixwebadm.py" "${PKIXWA_DIST_STAGE}/pkixwebadm.py"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_LIST_DIR}/VERSION" "${PKIXWA_DIST_STAGE}/VERSION"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/pkixwebadm" "${PKIXWA_DIST_STAGE}/pkixwebadm"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${PKIXWA_BUILD_DIR}" "${PKIXWA_DIST_STAGE}/build"
    COMMAND "${CMAKE_COMMAND}" -E tar "cfvz" "${PKIXWA_DIST_DIR}/${PKIXWA_DIST_NAME}.tar.gz" --format=gnutar -C "${PKIXWA_DIST_DIR}" "${PKIXWA_DIST_NAME}"
    DEPENDS pkixwebadm_build
    COMMENT "Packaging pkixwebadm ${PKIXWA_VERSION}"
)

add_custom_target(pkixwebadm_all
    DEPENDS pkixwebadm_build)

add_custom_target(pkixwebadm_example_demo
    COMMAND "${CMAKE_COMMAND}" -E echo "No pkixwebadm examples are defined.")
