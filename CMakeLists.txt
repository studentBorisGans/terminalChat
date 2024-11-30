# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TerminalChat VERSION 1.0)

# Set the C standard (e.g., C11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories for header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files for client and server
set(CLIENT_SRC
    src/client.c
    src/network_utils.c
)

set(SERVER_SRC
    src/server.c
    src/network_utils.c
)

# Add executables for the client and server
add_executable(Client ${CLIENT_SRC})
add_executable(Server ${SERVER_SRC})

# Optional: Link additional libraries if needed
target_link_libraries(Client pthread)  # Add `pthread` if threading is used in the client
target_link_libraries(Server pthread)  # Add `pthread` for server threading support

# Set build output directories for executables
set_target_properties(Client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(Server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
