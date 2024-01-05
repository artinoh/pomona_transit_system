# Minimum required version of CMake
cmake_minimum_required(VERSION 3.21)

# Project name
project(pomona_transit_system)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ODBC REQUIRED)
# Gather all source files in the utils directory
file(GLOB UTILS utils/*.cpp utils/*.h)

# Add the source files to the executable
add_executable(pomona_transit_system main.cpp ${UTILS})

target_link_libraries(pomona_transit_system PRIVATE ODBC::ODBC)



target_link_libraries(pomona_transit_system PRIVATE ODBC::ODBC)