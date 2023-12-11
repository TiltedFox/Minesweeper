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
                       int c, int r_count, int c_count)
    : Button(xy, (500 / r_count), (500 / c_count), "", cb), row{r}, column{c} {}

void CellButton::attach(Graph_lib::Window &win) { Button::attach(win); }

Game::Game(App *app, minesweeper::game_logic::Settings settings)
    : AppState{app}, field{settings},
      quit{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                            app->y_max() * 7 / 8},
           app->x_max() / 4, app->y_max() / 10, "Quit",
           [](Graph_lib::Address, Graph_lib::Address button_addr) {
             App &app = get_app_ref(button_addr);
             app.set_state(new Main_menu(&app));
           }},
      rec{Graph_lib::Point(350, 150), Graph_lib::Point(750, 450)},
      lose_text{Graph_lib::Point(500,20), "you are dick"}

{
  for (int i = 0; i < settings.count_rows; ++i)
    for (int j = 0; j < settings.count_columns; ++j) {
      int size = settings.count_rows < settings.count_columns
                     ? settings.count_rows
                     : settings.count_columns;
      cells.push_back(new CellButton{
          Graph_lib::Point{300 + j * (500 / size),
                           50 + (settings.count_rows - 1 - i) * (500 / size)},
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            auto widget_p = (static_cast<CellButton *>(button_addr));
            App &app = get_app_ref(button_addr);
            Game &game = dynamic_cast<Game &>(app.get_state());

            game.on_click(widget_p);
          },
          i, j, settings.count_rows, settings.count_columns});
    }
}

void Game::enter() {
  for (int i = 0; i < cells.size(); i++) {
    app->attach(cells[i]);
  }
}

void Game::exit() { 
  dettach_all_cells(); 
  dettach_all_number();
  app->detach(quit);
  app->detach(lose_text);
}

void Game::on_click(CellButton *btn) {
  minesweeper::game_logic::Field &field = this->field;
  minesweeper::game_logic::IndexPair cell = btn->get_index();
  if (field.get().empty()) {
    field.generate_field(cell);
    attach_number_from_field();
  }

  field.open_cell(cell);

  this->update();

  if (field.is_bomb(cell)) {
    dettach_all_cells();
    open_all_number();
    attach_lose_state();
    quit.hide();
    quit.show();
  }
}

void Game::update() {
  dettach_all_cells();
  attach_all_from_field();
}

void Game::attach_lose_state() {
  // rec.set_fill_color(Graph_lib::Color::red);
  app->attach(quit);
  lose_text.set_font_size(30);
  app->attach(lose_text);
}

void Game::attach_all_from_field() {
  for (int i = 0; i < cells.size(); i++) {
    if (!field.is_open(cells[i].get_index()))
      app->attach(cells[i]);
    else
      app->attach(Numbers[i]);
    // if
    // (field.get().at(cells[i].get_index().row).at(cells[i].get_index().column).count_bomb
    // == 9){
    //   std::cout << "boom";
    // }
  }
}

void Game::attach_number_from_field() {
  int r = field.get().size();
  int c = field.get()[0].size();
  int size = r < c ? r : c;

  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      std::string val = std::to_string(field.get()[i][j].count_bomb);
      if (val == "9") {
        val = "*";
      }
      if (val == "0") {
        val = "";
      }

      Numbers.push_back(
          new Graph_lib::Text(Graph_lib::Point{320 + j * (500 / size),
                                               80 + (r - 1 - i) * (500 / size)},
                              val));
    }
  }
  // Numbers.push_back(new Graph_lib::Text( Graph_lib::Point{400,200},
  // "GGGGGGGGGG" ));
}
void Game::open_all_number() {
  for (int i = 0; i < Numbers.size(); i++) {
    app->attach(Numbers[i]);
  }
}
void Game::dettach_all_cells() {
  for (int i = 0; i < cells.size(); i++) {
    app->detach(cells[i]);
  }
}

void Game::dettach_all_number() {
  for (int i = 0; i < Numbers.size(); i ++){
    app->detach(Numbers[i]);
  }
}

} // namespace minesweeper::app