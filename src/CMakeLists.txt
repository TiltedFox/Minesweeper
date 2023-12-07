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
    ${APP_DIR}/app_states.h
    ${APP_DIR}/app_states.cpp
)
target_link_libraries(app
    PUBLIC
    game_logic
    Graph_lib
)
add_executable(minesweeper
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(minesweeper
    app
)