#
# Usage:
# cmake -S . -B build -G Ninja
# cmake --build build
# cmake --build build --target libdmotservices_java_dist
# cmake --build build --target libdmotservices_java_test
#
# Notes:
# - Requires Ant and Maven on PATH; JVM location can be overridden via -DLDS_JAVA_HOME=/path.
# - Targets mirror the Ant build.xml: dist, test-compile, test, example-demo.

cmake_minimum_required(VERSION 3.16)

project(libdmotservices_java LANGUAGES NONE)

file(READ "${CMAKE_CURRENT_LIST_DIR}/../VERSION" LIBDMOTSERVICES_JAVA_VERSION_RAW)
string(STRIP "${LIBDMOTSERVICES_JAVA_VERSION_RAW}" LIBDMOTSERVICES_JAVA_VERSION)

find_program(LDS_ANT_EXECUTABLE ant)
find_program(LDS_MVN_EXECUTABLE mvn)

if(NOT LDS_ANT_EXECUTABLE)
    message(FATAL_ERROR "Ant not found; install it or set LDS_ANT_EXECUTABLE.")
endif()

if(NOT LDS_MVN_EXECUTABLE)
    message(FATAL_ERROR "Maven (mvn) not found; install it or set LDS_MVN_EXECUTABLE.")
endif()

if(DEFINED ENV{JAVA_HOME})
    set(LDS_JAVA_HOME_DEFAULT "$ENV{JAVA_HOME}")
else()
    set(LDS_JAVA_HOME_DEFAULT "/usr/lib/jvm/java-17-openjdk-amd64")
endif()

set(LDS_JAVA_HOME "${LDS_JAVA_HOME_DEFAULT}"
    CACHE PATH "JAVA_HOME to use for libdmotservices Java/Ant builds.")
set(LDS_JAVA_EXECUTABLE "${LDS_JAVA_HOME}/bin/java")

if(NOT EXISTS "${LDS_JAVA_EXECUTABLE}")
    message(FATAL_ERROR "Java executable not found at ${LDS_JAVA_EXECUTABLE}. Set LDS_JAVA_HOME.")
endif()

set(LDS_ANT_BUILD_FILE "${CMAKE_CURRENT_LIST_DIR}/build.xml")
set(LDS_ANT_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/build-ant")
set(LDS_VERSION_TOOL_DIR "${CMAKE_CURRENT_LIST_DIR}/tools")

add_custom_target(libdmotservices_java_version
    WORKING_DIRECTORY "${LDS_VERSION_TOOL_DIR}"
    COMMAND "${LDS_JAVA_EXECUTABLE}" VersionManager.java ../..
    COMMENT "Stamping POMs with VERSION (VersionManager.java)"
    VERBATIM)

function(_libdmotservices_java_target target ant_goal comment)
    add_custom_target(${target}
        DEPENDS libdmotservices_java_version
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
        COMMAND "${LDS_ANT_EXECUTABLE}"
                -f "${LDS_ANT_BUILD_FILE}"
                "-Dlibdmotservices.version=${LIBDMOTSERVICES_JAVA_VERSION}"
                "-Dproject.java.home=${LDS_JAVA_HOME}"
                "-Dmvn=${LDS_MVN_EXECUTABLE}"
                "-Dbuild=${LDS_ANT_BUILD_DIR}"
                "-Dmvn.skip.tests=-DskipTests"
                ${ant_goal}
        COMMENT "${comment}"
        VERBATIM)
endfunction()

_libdmotservices_java_target(libdmotservices_java_dist dist
    "Building libdmotservices Java distribution (Ant dist)")
_libdmotservices_java_target(libdmotservices_java_test_compile test-compile
    "Compiling libdmotservices Java tests (Ant test-compile)")
_libdmotservices_java_target(libdmotservices_java_test test
    "Running libdmotservices Java tests (Ant test)")
_libdmotservices_java_target(libdmotservices_java_example_demo example-demo
    "Running libdmotservices Java example demos (stub)")

add_custom_target(libdmotservices_java_all
    DEPENDS libdmotservices_java_dist)
