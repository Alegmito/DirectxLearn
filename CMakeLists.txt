cmake_minimum_required(VERSION 3.24)

project(DirectxLearn)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_STANDARD 23)
# find_package(directxmath CONFIG REQUIRED)
find_package(directxtk CONFIG REQUIRED)

add_subdirectory(src)
