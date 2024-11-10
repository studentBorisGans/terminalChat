# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TerminalChat VERSION 1.0)

# Set the C standard (e.g., C11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Create an executable from the source files
add_executable(Client client.c)
add_executable(Server server.c)

# Include directories (optional, if you have header files in subdirectories)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add any dependencies or link any libraries (optional)
target_link_libraries(Client m) # Example for linking math library (-lm)
target_link_libraries(Server m) # Example for linking math library (-lm)
