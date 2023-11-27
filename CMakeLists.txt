cmake_minimum_required(VERSION 3.20)

project("Minesweeper" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/Graph_lib) 
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)