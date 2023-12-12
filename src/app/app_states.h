#ifndef APP_STATES_H
#define APP_STATES_H

#include <app/app.h>

namespace minesweeper::app {

class Main_menu;
class Singleplayer_menu;
class Multiplayer_menu;
class Game_menu;

class Main_menu : public AppState {
public:
  Main_menu(App *app);

  void enter() override;
  void exit() override;

private:
  Graph_lib::Button singleplayer_button;
  Graph_lib::Button multiplayer_button;
  Graph_lib::Button quit;
  Graph_lib::Text header;
};

class Singleplayer_menu : public AppState {
public:
  Singleplayer_menu(App *app);

  void enter() override;
  void exit() override;

private:
  Graph_lib::Button easy_dif;   // Singleplayer menu,
  Graph_lib::Button medium_dif; // Buttons and boxes for choosing difficulty
  Graph_lib::Button hard_dif;
  Graph_lib::Button custom_dif;
  Graph_lib::In_box area_box;
  Graph_lib::In_box mines_box;
  Graph_lib::Button start_game_button;
};

class Multiplayer_menu : public AppState {
public:
  Multiplayer_menu(App *app);

  void enter() override;
  void exit() override;

private:
  Graph_lib::In_box ip_box;
  Graph_lib::Button start_game_button;
};

class Game_menu : public AppState {
public:
  Game_menu(App *app);

  void enter() override;
  void exit() override;

private:
  Graph_lib::Text test_text;
};

class Test : public AppState {
public:
  Test(App *app);

  void enter() override {
    app->attach(test_button);
    app->attach(mp_client_button);
    app->attach(mp_server_button);
  };
  void exit() override {
    app->detach(test_button);
    app->detach(mp_client_button);
    app->detach(mp_server_button);
  }

  ;

private:
  Graph_lib::Button test_button;
  Graph_lib::Button mp_server_button;
  Graph_lib::Button mp_client_button;
};

} // namespace minesweeper::app

#endif // #ifndef APP_STATES_H