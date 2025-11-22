#
# Usage:
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build
#   ctest --test-dir build/linux
#   cmake --build build --target txrxcli_dotnet_test

cmake_minimum_required(VERSION 3.15)
project(txrxcli LANGUAGES CXX)

# Linux dual-mode binary
add_subdirectory(linux)

# Windows .NET dual-mode binary (targets defined in windows/CMakeLists.txt)
add_subdirectory(windows)
