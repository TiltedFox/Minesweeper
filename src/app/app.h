#ifndef APP_H
#define APP_H

#include <string>

#include <Graph_lib/GUI.h>
#include <Graph_lib/Window.h>

#include <game_logic/game_logic.h>

<<<<<<< HEAD
#include <Graph_lib/Graph.h>
#include <Graph_lib/Window.h>
#include <Graph_lib/GUI.h>
=======
#include <iostream>
>>>>>>> dev

namespace minesweeper::app {
class App;

class AppState {
public:
  virtual void enter(App *app) = 0;
  virtual void exit(App *app) = 0;
  virtual ~AppState(){};
};

<<<<<<< HEAD
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

=======
class App : public Graph_lib::Window {
public:
  App(int w, int h, const std::string &title);

  void set_state(AppState &new_state);

private:
  AppState *current_state;
};

inline App &get_app_ref(Graph_lib::Address widget_address) {
  auto *widget_p = static_cast<Graph_lib::Widget *>(widget_address);
  return static_cast<App &>(widget_p->window());
}

} // namespace minesweeper::app
>>>>>>> dev
#endif // #ifndef GAME_H