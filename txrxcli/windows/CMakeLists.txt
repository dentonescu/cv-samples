# Usage:
#   cmake -S . -B build -G Ninja
#   cmake --build build --target txrxcli_dotnet_publish
#   cmake --build build --target txrxcli_dotnet_run
#   cmake --build build --target txrxcli_dotnet_test

cmake_minimum_required(VERSION 3.16)
project(txrxcli-dotnet NONE)

# Restore all NuGet packages
add_custom_target(txrxcli_dotnet_restore
    COMMAND ${CMAKE_COMMAND} -E env DOTNET_CLI_HOME=${CMAKE_CURRENT_LIST_DIR}
            dotnet restore ${CMAKE_CURRENT_LIST_DIR}/txrxcli-dotnet/txrxcli-dotnet.csproj
    COMMAND ${CMAKE_COMMAND} -E env DOTNET_CLI_HOME=${CMAKE_CURRENT_LIST_DIR}
            dotnet restore ${CMAKE_CURRENT_LIST_DIR}/txrxcli-dotnet/tests/txrxcli-dotnet.Tests.csproj
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/txrxcli-dotnet
    COMMENT "Restoring NuGet packages for txrxcli-dotnet")

# Publish win-x64 self-contained binary
add_custom_target(txrxcli_dotnet_publish
    COMMAND ${CMAKE_COMMAND} -E env DOTNET_CLI_HOME=${CMAKE_CURRENT_LIST_DIR}
            dotnet publish ${CMAKE_CURRENT_LIST_DIR}/txrxcli-dotnet/txrxcli-dotnet.csproj
            -c Release -r win-x64 --self-contained true
            -o ${CMAKE_BINARY_DIR}/windows/publish
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/txrxcli-dotnet
    COMMENT "Publishing txrxcli .NET win-x64 self-contained executable")

# Run via dotnet (adjust args by editing here or passing DOTNET_ARGS)
set(DOTNET_ARGS "--mode server --port 9000 --key SECRET" CACHE STRING "Args passed to dotnet run")
add_custom_target(txrxcli_dotnet_run
    COMMAND ${CMAKE_COMMAND} -E env DOTNET_CLI_HOME=${CMAKE_CURRENT_LIST_DIR}
            dotnet run --project ${CMAKE_CURRENT_LIST_DIR}/txrxcli-dotnet/txrxcli-dotnet.csproj -- ${DOTNET_ARGS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/txrxcli-dotnet
    COMMENT "Running txrxcli .NET via dotnet run")

# Default build target for this directory
add_custom_target(txrxcli_windows ALL DEPENDS txrxcli_dotnet_publish)

# Tests
add_custom_target(txrxcli_dotnet_test
    COMMAND ${CMAKE_COMMAND} -E env DOTNET_CLI_HOME=${CMAKE_CURRENT_LIST_DIR}
            dotnet test ${CMAKE_CURRENT_LIST_DIR}/txrxcli-dotnet/tests/txrxcli-dotnet.Tests.csproj -c Release --no-build -l "console;verbosity=detailed"
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/txrxcli-dotnet
    COMMENT "Running txrxcli .NET tests (requires NuGet access)")

add_dependencies(txrxcli_dotnet_publish txrxcli_dotnet_restore)
add_dependencies(txrxcli_dotnet_test txrxcli_dotnet_restore)
