#include <app/app_states.h>
#include <iostream>
namespace minesweeper::app {

Main_menu::Main_menu(App *app)
    : AppState{app},
      singleplayer_button{
          Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                           app->y_max() / 2},
          app->x_max() / 4, app->y_max() / 10, "Singleplayer",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            app.set_state(new Singleplayer_menu{&app});
          }},
      multiplayer_button{
          Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                           app->y_max() * 2 / 3},
          app->x_max() / 4, app->y_max() / 10, "Multiplayer",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            app.set_state(new Multiplayer_menu{&app});
          }},
      quit{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                            app->y_max() * 5 / 6},
           app->x_max() / 4, app->y_max() / 10, "Quit",
           [](Graph_lib::Address, Graph_lib::Address button_addr) {
             App &app = get_app_ref(button_addr);
             app.hide();
           }},
      header{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                              app->y_max() / 3},
             "Minesweeper"} {};

void Main_menu::enter() {
  app->attach(singleplayer_button);
  app->attach(multiplayer_button);
  app->attach(quit);
  header.set_font_size(40);
  header.set_color(Graph_lib::Color::black);
  app->attach(header);
}

void Main_menu::exit() {
  app->detach(singleplayer_button);
  app->detach(multiplayer_button);
  app->detach(quit);
  app->detach(header);
}

Singleplayer_menu::Singleplayer_menu(App *app)
    : AppState{app},
      easy_dif{Graph_lib::Point{app->x_max() / 5, app->y_max() / 3},
               app->x_max() / 5, app->y_max() / 10, "Easy",
               [](Graph_lib::Address, Graph_lib::Address button_addr) {
                 App &app = get_app_ref(button_addr);
                 app.set_state(new Game{&app, minesweeper::game_logic::kEasy});
               }},
      medium_dif{
          Graph_lib::Point{app->x_max() / 5, app->y_max() / 2},
          app->x_max() / 5, app->y_max() / 10, "Medium",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            app.set_state(new Game{&app, minesweeper::game_logic::kMedium});
          }},
      hard_dif{Graph_lib::Point{app->x_max() / 5, app->y_max() * 2 / 3},
               app->x_max() / 5, app->y_max() / 10, "Hard",
               [](Graph_lib::Address, Graph_lib::Address button_addr) {
                 App &app = get_app_ref(button_addr);
                 app.set_state(new Game{&app, minesweeper::game_logic::kHard});
               }},
      custom_dif{Graph_lib::Point{app->x_max() * 3 / 5, app->y_max() / 3},
                 app->x_max() / 5, app->y_max() / 10, "Custom",
                 [](Graph_lib::Address, Graph_lib::Address button_addr) {
                   App &app = get_app_ref(button_addr);
                   app.set_state(new Game{
                       &app, minesweeper::game_logic::Settings{10, 10, 30}});
                 }},
      area_box{Graph_lib::Point{app->x_max() * 3 / 5, app->y_max() / 2},
               app->x_max() / 5, app->y_max() / 10, "Area"},
      mines_box{Graph_lib::Point{app->x_max() * 3 / 5, app->y_max() * 2 / 3},
                app->x_max() / 5, app->y_max() / 10, "Mines"},
      start_game_button{
          Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                           app->y_max() * 5 / 6},
          app->x_max() / 4, app->y_max() / 10, "Start game",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            app.set_state(
                new Game{&app, minesweeper::game_logic::Settings{10, 10, 30}});
          }} {};

void Singleplayer_menu::enter() {
  app->attach(easy_dif);
  app->attach(medium_dif);
  app->attach(hard_dif);
  app->attach(custom_dif);
  app->attach(area_box);
  app->attach(mines_box);
  app->attach(start_game_button);
}

void Singleplayer_menu::exit() {
  app->detach(easy_dif);
  app->detach(medium_dif);
  app->detach(hard_dif);
  app->detach(custom_dif);
  app->detach(area_box);
  app->detach(mines_box);
  app->detach(start_game_button);
}

Multiplayer_menu::Multiplayer_menu(App *app)
    : AppState(app),
      ip_box{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                              app->y_max() * 2 / 3},
             app->x_max() / 4, app->y_max() / 10, "Ip-adress"},
      start_game_button{
          Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                           app->y_max() * 5 / 6},
          app->x_max() / 4, app->y_max() / 10, "Start game",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            app.set_state(new Game{&app, minesweeper::game_logic::kEasy});
          }} {};

void Multiplayer_menu::enter() {
  app->attach(ip_box);
  app->attach(start_game_button);
}

void Multiplayer_menu::exit() {
  app->detach(ip_box);
  app->detach(start_game_button);
}

CellButton::CellButton(Graph_lib::Point xy, Graph_lib::Callback cb, int r,
                       int c)
    : Button(xy, size, size, "", cb), row{r}, column{c} {}

void CellButton::attach(Graph_lib::Window &win) { Button::attach(win); }

Game::Game(App *app, minesweeper::game_logic::Settings settings)
    : AppState{app}, field{settings} {
  for (int i = 0; i < settings.count_rows; ++i)
    for (int j = 0; j < settings.count_columns; ++j) {
      cells.push_back(new CellButton{
          Graph_lib::Point{20 + j * CellButton::size,
                           20 + (settings.count_rows - 1 - i) *
                                    CellButton::size},
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            auto widget_p = (static_cast<CellButton *>(button_addr));
            App &app = get_app_ref(button_addr);
            Game &game = dynamic_cast<Game &>(app.get_state());

            game.on_click(widget_p);
          },
          i, j});
    }
}

void Game::enter() {
  for (int i = 0; i < cells.size(); i++) {
    app->attach(cells[i]);
  }
}

void Game::exit() { dettach_all(); }

void Game::on_click(CellButton *btn) {
  minesweeper::game_logic::Field &field = this->field;
  minesweeper::game_logic::IndexPair cell = btn->get_index();
  if (field.get().empty())
    field.generate_field(cell);
    attach_number_from_field();

  field.open_cell(cell);

  this->update();
}

void Game::update() {
  dettach_all();
  attach_all_from_field();
}

void Game::attach_all_from_field() {
  for (int i = 0; i < cells.size(); i++) {
    if (!field.is_open(cells[i].get_index()))  
      app->attach(cells[i]);
    else
      app->attach(Numbers[i]);
  }
}

void Game::attach_number_from_field() {
  int r = field.get().size();
  int c = field.get()[0].size();

  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      int val = field.get()[i][j].count_bomb;
      Numbers.push_back(new Graph_lib::Text( Graph_lib::Point{40 + j * CellButton::size,
                           50 + (r - 1 - i) *
                                    CellButton::size}, std::to_string(val) ));
    }
  }
}

void Game::dettach_all() {
  for (int i = 0; i < cells.size(); i++) {
    app->detach(cells[i]);
  }
}

} // namespace minesweeper::app