#ifndef APP_STATES_H
#define APP_STATES_H
#include <vector>
#include <app/app.h>

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




struct Button_Cell : Graph_lib::Button {
  Button_Cell(Graph_lib::Point xy, Graph_lib::Callback cb, int r, int c);



  void attach (Graph_lib::Window& win) override;
  minesweeper::game_logic::IndexPair get_user_click(){
    return minesweeper::game_logic::IndexPair(row, column);
  }

  static constexpr int size = 50;
  private:
    int row;
    int column;
};

class Game : public AppState {
public:
  Game(App *app);

  void enter() override;
  void exit() override;

private:
  Graph_lib::Vector_ref <Button_Cell> cells;
  minesweeper::game_logic::Field* field;
  static void clicked(Button_Cell* c);
};
} // namespace minesweeper::app

#endif // #ifndef APP_STATES_H