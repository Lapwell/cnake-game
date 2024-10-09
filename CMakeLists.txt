# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CnakeMan)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify the C standard (C23 as you mentioned earlier)
set(CMAKE_C_STANDARD 23)

set(CMAKE_C_EXTENSIONS OFF)

# Include the header files
include_directories(include)

# Add the executable (main source file)
add_executable(cnake-man src/main.c src/level_loader.c src/traverse_level.c)

# Link against the static library
target_link_libraries(cnake-man ${CMAKE_SOURCE_DIR}/lib/libraylib.a)

# Optionally, add custom compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2 -pedantic -Wextra -Wshadow -g -lraylib -lGL -lm -lpthread -ldl -lrt -lX11")

