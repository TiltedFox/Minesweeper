#ifndef APP_STATES_H
#define APP_STATES_H
#include <app/app.h>
#include <vector>

namespace minesweeper::app {

class Main_menu;
class Singleplayer_menu;
class Multiplayer_menu;

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

struct CellButton : Graph_lib::Button {
  CellButton(Graph_lib::Point xy, Graph_lib::Callback cb, int r, int c,
             int r_count, int c_count);

  void attach(Graph_lib::Window &win) override;
  minesweeper::game_logic::IndexPair get_index() {
    return minesweeper::game_logic::IndexPair(row, column);
  }

private:
  int row;
  int column;
};

class Game : public AppState {
public:
  Game(App *app, minesweeper::game_logic::Settings settings);

  void enter() override;
  void exit() override;

private:
  Graph_lib::Text lose_text;
  Graph_lib::Rectangle rec;
  Graph_lib::Vector_ref<CellButton> cells;
  Graph_lib::Vector_ref<Graph_lib::Text> Numbers;
  minesweeper::game_logic::Field field;
  Graph_lib::Button quit;
  void attach_lose_state();
  void init_buttons();
  void update();
  void attach_all_from_field();
  void attach_number_from_field();
  void dettach_all_number();
  void dettach_all_cells();
  void open_all_number();
  void on_click(CellButton *btn);
};

} // namespace minesweeper::app

#endif // #ifndef APP_STATES_H