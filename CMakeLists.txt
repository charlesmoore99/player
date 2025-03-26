cmake_minimum_required(VERSION 3.15)
project(player CXX)

# Enable C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXE_LINKER_FLAGS "-static")

find_package(fmt REQUIRED)
find_package(RapidJSON REQUIRED)

include_directories("${RapidJSON_INCLUDE_DIR}")

add_executable(player 
    src/main.cpp
    src/Player.cpp
)
target_link_libraries(player PRIVATE fmt::fmt)