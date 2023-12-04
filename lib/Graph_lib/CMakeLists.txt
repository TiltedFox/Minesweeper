set(FLTK_SKIP_FLUID True)
set(OpenGL_GL_PREFERENCE "GLVND")

find_package(FLTK REQUIRED)
find_package(OpenGL REQUIRED)

set(HEADERS
    fltk.h
    Graph.h
    GUI.h
    Point.h
    Window.h
    Simple_window.h
)

set(SOURCES
    Graph.cpp
    GUI.cpp
    Window.cpp
)

add_library(Graph_lib
    ${HEADERS}
    ${SOURCES}
)


target_include_directories(Graph_lib
    PUBLIC
    ${FLTK_INCLUDE_DIR}
    ../
)

target_link_directories(Graph_lib
    PUBLIC
    ${FLTK_INCLUDE_DIR}/../lib
)

target_link_libraries(Graph_lib
    ${FLTK_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

if(WIN32)
target_link_libraries(Graph_lib
    fltk
    fltk_jpeg
    fltk_png
    fltk_z
)
endif()