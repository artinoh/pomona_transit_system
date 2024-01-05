#!/bin/bash

# Set the build directory
BUILD_DIR="/Users/artin/CS4350/lab4/pomona_transit_system/build"

# Change to the build directory
cd "$BUILD_DIR" || exit

# Build the project using CMake
cmake ..

# Compile the project
make

# Run the executable
./pomona_transit_system
