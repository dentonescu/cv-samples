#
# Usage:
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build
#   ctest --test-dir build

cmake_minimum_required(VERSION 3.15)
project(txrxcli LANGUAGES CXX)

# Linux dual-mode binary
add_subdirectory(linux)
