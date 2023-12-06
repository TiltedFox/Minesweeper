#ifndef APP_STATES_H
#define APP_STATES_H

#include <app/app.h>

namespace minesweeper::app {

class Main_menu : public AppState {
public:
  static AppState &get_instance() {
    static Main_menu singleton;
    return singleton;
  }

  void enter(App *app) override;
  void exit(App *app) override;

private:
  Main_menu();
  Graph_lib::Button singleplayer_button;
  Graph_lib::Button multiplayer_button;
  Graph_lib::Button quit;
  Graph_lib::Text header;
};

class Singleplayer_menu : public AppState {
public:
  static AppState &get_instance() {
    static Singleplayer_menu singleton;
    return singleton;
  }

  void enter(App *app) override;
  void exit(App *app) override;

private:
  Singleplayer_menu();
  Graph_lib::Button easy_dif;         //Singleplayer menu,
  Graph_lib::Button medium_dif;       //Buttons and boxes for choosing difficulty
  Graph_lib::Button hard_dif;
  Graph_lib::Button custom_dif;
  Graph_lib::In_box area_box;
  Graph_lib::In_box mines_box;
  Graph_lib::Button start_game_button;
};

class Multiplayer_menu : public AppState {
public:
  static AppState &get_instance() {
    static Multiplayer_menu singleton;
    return singleton;
  }

  void enter(App *app) override;
  void exit(App *app) override;

private:
  Multiplayer_menu();
  Graph_lib::In_box ip_box;
  Graph_lib::Button start_game_button;
};

class Game_menu : public AppState {
public:
  static AppState &get_instance() {
    static Game_menu singleton;
    return singleton;
  }

  void enter(App *app) override {};
  void exit(App *app) override {};

private:
  Game_menu();
};
} // namespace minesweeper::app

#endif // #ifndef APP_STATES_H