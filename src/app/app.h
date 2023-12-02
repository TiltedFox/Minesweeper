#ifndef APP_H
#define APP_H

#include <game_logic/game_logic.h>

#include <Graph_lib/Graph.h>
#include <Graph_lib/Window.h>
#include <Graph_lib/GUI.h>

namespace minesweeper::app {
class App;

} // namespace minesweeper::app

struct Win_window : Graph_lib::Window {
    Win_window(Graph_lib::Point xy, int w, int h, int button_w, int button_h, const std::string& title);

    private:
    Graph_lib::Button singleplayer_button;      //main menu buttons
    Graph_lib::Button multiplayer_button;
    Graph_lib::Button quit_button;
    Graph_lib::Text header;

    Graph_lib::Button easy_dif;         //Singleplayer menu,
    Graph_lib::Button medium_dif;       //Buttons and boxes for choosing difficulty
    Graph_lib::Button hard_dif;
    Graph_lib::Button custom_dif;
    Graph_lib::In_box area_box;
    Graph_lib::In_box mines_box;
    Graph_lib::Button start_game_button;

    Graph_lib::In_box ip_box;           //Multiplayer menu

    static void cb_quit(Graph_lib::Address, Graph_lib::Address);
    static void cb_singleplayer(Graph_lib::Address, Graph_lib::Address);
    static void cb_multiplayer(Graph_lib::Address, Graph_lib::Address);

    static void cb_easy(Graph_lib::Address, Graph_lib::Address);
    static void cb_medium(Graph_lib::Address, Graph_lib::Address);
    static void cb_hard(Graph_lib::Address, Graph_lib::Address);
    static void cb_custom(Graph_lib::Address, Graph_lib::Address);
    static void cb_area(Graph_lib::Address, Graph_lib::Address);
    static void cb_mines(Graph_lib::Address, Graph_lib::Address);
    static void cb_start_game(Graph_lib::Address, Graph_lib::Address);

    static void cb_ip(Graph_lib::Address, Graph_lib::Address);

    void quit();
    void singleplayer();
    void multiplayer();

    void easy();
    void medium();
    void hard();
    void custom();
    void start_game();

    void ip();
};

#endif // #ifndef GAME_H