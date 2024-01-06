# Minimum required version of CMake
cmake_minimum_required(VERSION 3.21)

# Project name
project(pomona_transit_system)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ODBC REQUIRED)
find_package(Boost REQUIRED)

file(GLOB UTILS utils/src/*.cpp utils/*.h)
file(GLOB TRANSIT_MENU transit_menu/src/*.cpp transit_menu/*.h)



add_executable(pomona_transit_system main.cpp ${UTILS} ${TRANSIT_MENU})


target_link_libraries(pomona_transit_system PRIVATE ODBC::ODBC Boost::boost)
