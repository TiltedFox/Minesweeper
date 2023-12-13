set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS json)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GAME_LOGIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/game_logic")
add_library(game_logic
    ${GAME_LOGIC_DIR}/game_logic.h
    ${GAME_LOGIC_DIR}/game_logic.cpp
)

set(APP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/app")
add_library(app
    ${APP_DIR}/app.h
    ${APP_DIR}/app.cpp
    ${APP_DIR}/menu_states.h
    ${APP_DIR}/menu_states.cpp
    ${APP_DIR}/ingame_states.h
    ${APP_DIR}/ingame_states.cpp
)
if(WIN32)
  target_link_libraries(app
  PUBLIC
  wsock32
  ws2_32
)
endif()
target_include_directories(app
    PUBLIC
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(app
    PUBLIC
    game_logic
    Graph_lib
    ${Boost_LIBRARIES}
)

add_executable(minesweeper
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(minesweeper
    app
)