cmake_minimum_required(VERSION 3.20)

set(TARGET "minesweeper")
project(${TARGET} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra")

cmake_path(SET LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
cmake_path(SET SRC ${CMAKE_SOURCE_DIR}/src)

include_directories(
    ${LIB_DIR}
)

set(SOURCES
    ${SRC}/main.cpp

    ${LIB_DIR}/Graph_lib/Graph.cpp
    ${LIB_DIR}/Graph_lib/GUI.cpp
    ${LIB_DIR}/Graph_lib/Window.cpp
    ${LIB_DIR}/Graph_lib/GraphShape.cpp
)

set(FLTK_SKIP_FLUID True)
set(FLTK_SKIP_FRMS True)

find_package(FLTK REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${TARGET}
    ${SOURCES}
)

target_include_directories(${TARGET}
    PRIVATE
    ${GRAPHLIB_DIR}
    SYSTEM
    ${FLTK_INCLUDE_DIR}
    ${LIB_DIR}
    ${SRC}
)

link_directories(
    ${FLTK_INCLUDE_DIR}/../lib
)

target_link_libraries(${TARGET}
    ${FLTK_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

if(WIN32)
    target_link_libraries(${TARGET}
        fltk_jpeg
        fltk_png
        fltk_z
    )
endif()

add_subdirectory(src)

install(TARGETS ${TARGET})