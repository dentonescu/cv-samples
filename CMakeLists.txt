#
# Usage:
# cmake -S . -B build -G Ninja
# cmake --build build
# cmake --build build --target cv_samples_dist
# cmake --build build --target cv_samples_test
# cmake --build build --target cv_samples_example_demo

cmake_minimum_required(VERSION 3.16)

project(cv_samples LANGUAGES C)

add_subdirectory(libdmotservices)
add_subdirectory(slideshow-server)
add_subdirectory(wifiequ)
add_subdirectory(pkixwebadm)
add_subdirectory(txrxcli)
# heapmonj docs (Gradle-based app; docs via CMake)
add_subdirectory(heapmonj)

find_program(CV_MAVEN_EXECUTABLE mvn)
if(NOT CV_MAVEN_EXECUTABLE)
    message(WARNING "mvn not found; libdmotservices Maven install will be skipped in CMake.")
endif()

set(CV_DIST_DIR "${CMAKE_CURRENT_LIST_DIR}/dist")

# gather artifacts
file(GLOB_RECURSE CV_ARCHIVES
     "${CMAKE_CURRENT_LIST_DIR}/*.a")
file(GLOB_RECURSE CV_TARBALLS
     "${CMAKE_CURRENT_LIST_DIR}/*.tar.gz")
file(GLOB_RECURSE CV_DIR_CANDIDATES LIST_DIRECTORIES true
     "${CMAKE_CURRENT_LIST_DIR}/*")

set(CV_BUILD_DIRS "")
set(CV_BIN_DIRS "")
foreach(path IN LISTS CV_DIR_CANDIDATES)
    if(path MATCHES "/node_modules/")
        continue()
    endif()
    if(IS_DIRECTORY "${path}")
        get_filename_component(_name "${path}" NAME)
        if(_name STREQUAL "build")
            list(APPEND CV_BUILD_DIRS "${path}")
        elseif(_name STREQUAL "bin")
            list(APPEND CV_BIN_DIRS "${path}")
        endif()
    endif()
endforeach()

add_custom_target(cv_samples_gather
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CV_DIST_DIR}"
    COMMENT "Gathering distribution artifacts across the workspace")

foreach(_archive IN LISTS CV_ARCHIVES)
    if(_archive MATCHES "/node_modules/")
        continue()
    endif()
    add_custom_command(TARGET cv_samples_gather
        COMMAND "${CMAKE_COMMAND}" -E copy "${_archive}" "${CV_DIST_DIR}"
        VERBATIM)
endforeach()

foreach(_tar IN LISTS CV_TARBALLS)
    if(_tar MATCHES "/node_modules/")
        continue()
    endif()
    add_custom_command(TARGET cv_samples_gather
        COMMAND "${CMAKE_COMMAND}" -E copy "${_tar}" "${CV_DIST_DIR}"
        VERBATIM)
endforeach()

foreach(_builddir IN LISTS CV_BUILD_DIRS)
    add_custom_command(TARGET cv_samples_gather
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${_builddir}" "${CV_DIST_DIR}"
        VERBATIM)
endforeach()

foreach(_bindir IN LISTS CV_BIN_DIRS)
    add_custom_command(TARGET cv_samples_gather
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${_bindir}" "${CV_DIST_DIR}"
        VERBATIM)
endforeach()

if(CV_MAVEN_EXECUTABLE)
    add_custom_target(libdmotservices_mvn_install
        WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/libdmotservices/java"
        COMMAND "${CV_MAVEN_EXECUTABLE}" -DskipTests install
        COMMENT "Installing libdmotservices into local Maven repo"
        VERBATIM)
endif()

add_custom_target(cv_samples_all)
if(TARGET libdmotservices_all)
    add_dependencies(cv_samples_all libdmotservices_all)
endif()
if(TARGET wifiequ_all)
    add_dependencies(cv_samples_all wifiequ_all)
endif()
if(TARGET slideshow_all)
    add_dependencies(cv_samples_all slideshow_all)
endif()
if(TARGET pkixwa_all)
    add_dependencies(cv_samples_all pkixwa_all)
endif()
if(TARGET txrxcli)
    add_dependencies(cv_samples_all txrxcli)
endif()
if(TARGET heapmonj_all)
    add_dependencies(cv_samples_all heapmonj_all)
elseif(TARGET heapmonj_docs_openapi)
    add_dependencies(cv_samples_all heapmonj_docs_openapi)
endif()
if(TARGET libdmotservices_mvn_install)
    add_dependencies(cv_samples_all libdmotservices_mvn_install)
endif()

add_custom_target(cv_samples_tests)
if(TARGET libdmotservices_tests)
    add_dependencies(cv_samples_tests libdmotservices_tests)
endif()
if(TARGET wifiequ_tests)
    add_dependencies(cv_samples_tests wifiequ_tests)
endif()
if(TARGET slideshow_test)
    add_dependencies(cv_samples_tests slideshow_test)
endif()
if(TARGET pkixwa_test)
    add_dependencies(cv_samples_tests pkixwa_test)
endif()
if(TARGET txrxcli_tests)
    add_dependencies(cv_samples_tests txrxcli_tests)
endif()
if(TARGET heapmonj_test)
    add_dependencies(cv_samples_tests heapmonj_test)
endif()

add_custom_target(cv_samples_test
    DEPENDS cv_samples_tests
    COMMENT "Executing workspace test suite")

add_custom_target(cv_samples_examples)
if(TARGET libdmotservices_examples)
    add_dependencies(cv_samples_examples libdmotservices_examples)
endif()
if(TARGET wifiequ_examples)
    add_dependencies(cv_samples_examples wifiequ_examples)
endif()

add_custom_target(cv_samples_example_demo)
if(TARGET libdmotservices_example_demo)
    add_dependencies(cv_samples_example_demo libdmotservices_example_demo)
endif()
if(TARGET wifiequ_example_demo)
    add_dependencies(cv_samples_example_demo wifiequ_example_demo)
endif()
if(TARGET slideshow_example_demo)
    add_dependencies(cv_samples_example_demo slideshow_example_demo)
endif()
if(TARGET pkixwa_example_demo)
    add_dependencies(cv_samples_example_demo pkixwa_example_demo)
endif()
if(TARGET heapmonj_example_demo)
    add_dependencies(cv_samples_example_demo heapmonj_example_demo)
endif()

add_custom_target(cv_samples_dist
    DEPENDS cv_samples_all cv_samples_gather
    COMMENT "Workspace distribution pipeline")
