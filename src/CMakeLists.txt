include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GAME_LOGIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/game_logic")
add_library(game_logic
    ${GAME_LOGIC_DIR}/game_logic.h
    ${GAME_LOGIC_DIR}/game_logic.cpp
)

set(APP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/app")
add_executable(app
    ${APP_DIR}/app.h
    ${APP_DIR}/app.cpp
)
target_link_libraries(app
    PRIVATE
    game_logic
    Graph_lib
)