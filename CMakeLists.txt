cmake_minimum_required(VERSION 3.15)
project(player CXX)

# Enable C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(fmt REQUIRED)

add_executable(player src/main.cpp)
target_link_libraries(player PRIVATE fmt::fmt)

