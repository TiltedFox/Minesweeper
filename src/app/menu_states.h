#ifndef APP_STATES_H
#define APP_STATES_H
#include <app/app.h>

namespace minesweeper::app {

class Main_menu;
class Singleplayer_menu;
class MP_client_menu;

class Main_menu : public AppState {
public:
  Main_menu(App *app);

  void enter() override;
  void exit() override;

private:
  Graph_lib::Button singleplayer_button; // Main menu
  Graph_lib::Button multiplayer_button;  // Buttons to navigate further
  Graph_lib::Button quit;
  Graph_lib::Text header;
};

class Singleplayer_menu : public AppState {
public:
  Singleplayer_menu(App *app);

  void enter() override;
  void exit() override;

  void error_attach();

private:
  Graph_lib::Button easy_dif;   // Singleplayer menu,
  Graph_lib::Button medium_dif; // Buttons and boxes for choosing difficulty
  Graph_lib::Button hard_dif;
  Graph_lib::Button custom_dif;
  Graph_lib::In_box area_box;
  Graph_lib::In_box mines_box;
  Graph_lib::Text input_condition;
  Graph_lib::Button back;
};

class Multiplayer_menu : public AppState {
public:
  Multiplayer_menu(App *app);

  void enter() override;
  void exit() override;

private:
  Graph_lib::Button client; // Multiplayer client menu
  Graph_lib::Button host;   // Buttons and boxes for connecting to host
  Graph_lib::Button back;
};

class MP_client_menu : public AppState {
public:
  MP_client_menu(App *app);

  void enter() override;
  void exit() override;

  std::string get_ip() { return ip_box.get_string(); }
  void set_label(std::string label) { error_text.set_label(label); }

private:
  Graph_lib::In_box ip_box; // Multiplayer client menu
  Graph_lib::Text error_text;
  Graph_lib::Button
      start_game_button; // Buttons and boxes for connecting to host
  Graph_lib::Button back;
};

class MP_host_menu : public AppState {
public:
  MP_host_menu(App *app);

  void enter() override;
  void exit() override;

private:
  Graph_lib::Button easy_dif;   // Multiplayer host menu,
  Graph_lib::Button medium_dif; // Buttons and boxes for choosing difficulty
  Graph_lib::Button hard_dif;
  Graph_lib::Button custom_dif;
  Graph_lib::In_box area_box;
  Graph_lib::In_box mines_box;
  Graph_lib::Text input_condition;
  Graph_lib::Button back;
};

} // namespace minesweeper::app

#endif // #ifndef APP_STATES_H