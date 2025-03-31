cmake_minimum_required(VERSION 3.15)
project(player CXX)

# Enable C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXE_LINKER_FLAGS "-static")

find_package(fmt REQUIRED)
find_package(RapidJSON REQUIRED)

include_directories("${fmt_INCLUDE_DIR}")
include_directories("${RapidJSON_INCLUDE_DIR}")

# Create a library instead of an executable
add_library(player_lib STATIC src/Player.cpp)
target_include_directories(player_lib PUBLIC include)

# Define the main executable
add_executable(player src/main.cpp)
target_link_libraries(player player_lib fmt::fmt)


# Find and link Catch2 for testing
find_package(Catch2 REQUIRED)
include_directories("${Catch2_INCLUDE_DIR}" src)

add_executable(test_player tests/test_player.cpp)
target_link_libraries(test_player PRIVATE player_lib Catch2::Catch2WithMain fmt::fmt dl)

# Enable CTest and auto-discover tests
include(CTest)
include(Catch)
catch_discover_tests(test_player)
