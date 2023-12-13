#include <app/app_states.h>
#include <app/ingame_states.h>

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
      easy_dif{Graph_lib::Point{app->x_max() * 3 / 14, app->y_max() / 3},
               app->x_max() / 7, app->y_max() / 10, "Easy",
               [](Graph_lib::Address, Graph_lib::Address button_addr) {
                 App &app = get_app_ref(button_addr);
                 app.set_state(new Game{&app, minesweeper::game_logic::kEasy});
               }},
      medium_dif{
          Graph_lib::Point{app->x_max() * 3 / 7, app->y_max() / 3},
          app->x_max() / 7, app->y_max() / 10, "Medium",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            app.set_state(new Game{&app, minesweeper::game_logic::kMedium});
          }},
      hard_dif{Graph_lib::Point{app->x_max() * 9 / 14, app->y_max() / 3},
               app->x_max() / 7, app->y_max() / 10, "Hard",
               [](Graph_lib::Address, Graph_lib::Address button_addr) {
                 App &app = get_app_ref(button_addr);
                 app.set_state(new Game{&app, minesweeper::game_logic::kHard});
               }},
      custom_dif{
          Graph_lib::Point{app->x_max() * 3 / 14, app->y_max() * 4 / 7},
          app->x_max() / 7, app->y_max() / 10, "Custom",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            Singleplayer_menu &menu =
                dynamic_cast<Singleplayer_menu &>(app.get_state());
            if (menu.area_box.get_int() > 20) {
            } else if (menu.mines_box.get_int() >
                       menu.area_box.get_int() * menu.area_box.get_int() / 2) {
            } else
              app.set_state(new Game{&app, minesweeper::game_logic::Settings{
                                               menu.area_box.get_int(),
                                               menu.area_box.get_int(),
                                               menu.mines_box.get_int()}});
          }},
      area_box{Graph_lib::Point{app->x_max() * 3 / 7, app->y_max() * 4 / 7},
               app->x_max() / 7, app->y_max() / 10, "Area"},
      mines_box{Graph_lib::Point{app->x_max() * 9 / 14, app->y_max() * 4 / 7},
                app->x_max() / 7, app->y_max() / 10, "Mines"},
      input_condition{Graph_lib::Point{app->x_max() / 3, app->y_max() * 3 / 4},
                      "Area must be <20 and mines < 2/3 * area"},
      back{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                            app->y_max() * 5 / 6},
           app->x_max() / 4, app->y_max() / 10, "Back",
           [](Graph_lib::Address, Graph_lib::Address button_addr) {
             App &app = get_app_ref(button_addr);
             app.set_state(new Main_menu{&app});
           }} {};

void Singleplayer_menu::enter() {
  app->attach(easy_dif);
  app->attach(medium_dif);
  app->attach(hard_dif);
  app->attach(custom_dif);
  app->attach(area_box);
  app->attach(mines_box);
  app->attach(input_condition);
  input_condition.set_font_size(20);
  app->attach(back);
}

void Singleplayer_menu::exit() {
  app->detach(easy_dif);
  app->detach(medium_dif);
  app->detach(hard_dif);
  app->detach(custom_dif);
  app->detach(area_box);
  app->detach(mines_box);
  app->detach(input_condition);
  app->detach(back);
}

Multiplayer_menu::Multiplayer_menu(App *app)
    : AppState(app),
      client{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                              app->y_max() / 2},
             app->x_max() / 4, app->y_max() / 10, "Join",
             [](Graph_lib::Address, Graph_lib::Address button_addr) {
               App &app = get_app_ref(button_addr);
               app.set_state(new MP_client_menu{&app});
             }},
      host{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                            app->y_max() * 2 / 3},
           app->x_max() / 4, app->y_max() / 10, "Host",
           [](Graph_lib::Address, Graph_lib::Address button_addr) {
             App &app = get_app_ref(button_addr);
             app.set_state(new MP_host_menu{&app});
           }},
      back{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                            app->y_max() * 5 / 6},
           app->x_max() / 4, app->y_max() / 10, "Back",
           [](Graph_lib::Address, Graph_lib::Address button_addr) {
             App &app = get_app_ref(button_addr);
             app.set_state(new Main_menu{&app});
           }} {};

void Multiplayer_menu::enter() {
  app->attach(client);
  app->attach(host);
  app->attach(back);
}

void Multiplayer_menu::exit() {
  app->detach(client);
  app->detach(host);
  app->detach(back);
}

MP_client_menu::MP_client_menu(App *app)
    : AppState(app),
      ip_box{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                              app->y_max() * 2 / 3},
             app->x_max() / 4, app->y_max() / 10, "Ip-adress"},
      start_game_button{
          Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                           app->y_max() * 5 / 6},
          app->x_max() / 4, app->y_max() / 10, "Connect",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            app.set_state(new Game{&app, minesweeper::game_logic::kEasy});
          }},
      back{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                            app->y_max() * 5 / 6},
           app->x_max() / 4, app->y_max() / 10, "Back",
           [](Graph_lib::Address, Graph_lib::Address button_addr) {
             App &app = get_app_ref(button_addr);
             app.set_state(new Multiplayer_menu{&app});
           }} {};

void MP_client_menu::enter() {
  app->attach(ip_box);
  app->attach(start_game_button);
  app->attach(back);
}

void MP_client_menu::exit() {
  app->detach(ip_box);
  app->detach(start_game_button);
  app->detach(back);
}

MP_host_menu::MP_host_menu(App *app)
    : AppState(app),
      easy_dif{Graph_lib::Point{app->x_max() * 3 / 14, app->y_max() / 3},
               app->x_max() / 7, app->y_max() / 10, "Easy",
               [](Graph_lib::Address, Graph_lib::Address button_addr) {
                 App &app = get_app_ref(button_addr);
                 app.set_state(new Game{&app, minesweeper::game_logic::kEasy});
               }},
      medium_dif{
          Graph_lib::Point{app->x_max() * 3 / 7, app->y_max() / 3},
          app->x_max() / 7, app->y_max() / 10, "Medium",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            app.set_state(new Game{&app, minesweeper::game_logic::kMedium});
          }},
      hard_dif{Graph_lib::Point{app->x_max() * 9 / 14, app->y_max() / 3},
               app->x_max() / 7, app->y_max() / 10, "Hard",
               [](Graph_lib::Address, Graph_lib::Address button_addr) {
                 App &app = get_app_ref(button_addr);
                 app.set_state(new Game{&app, minesweeper::game_logic::kHard});
               }},
      custom_dif{
          Graph_lib::Point{app->x_max() * 3 / 14, app->y_max() * 4 / 7},
          app->x_max() / 7, app->y_max() / 10, "Custom",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            MP_host_menu &menu = dynamic_cast<MP_host_menu &>(app.get_state());
            if (menu.area_box.get_int() > 20) {
            } else if (menu.mines_box.get_int() >
                       menu.area_box.get_int() * menu.area_box.get_int() / 2) {
            } else
              app.set_state(new Game{&app, minesweeper::game_logic::Settings{
                                               menu.area_box.get_int(),
                                               menu.area_box.get_int(),
                                               menu.mines_box.get_int()}});
          }},
      area_box{Graph_lib::Point{app->x_max() * 3 / 7, app->y_max() * 4 / 7},
               app->x_max() / 7, app->y_max() / 10, "Area"},
      mines_box{Graph_lib::Point{app->x_max() * 9 / 14, app->y_max() * 4 / 7},
                app->x_max() / 7, app->y_max() / 10, "Mines"},
      input_condition{Graph_lib::Point{app->x_max() / 3, app->y_max() * 3 / 4},
                      "Area must be < 20 and mines < 2/3 * area"},
      back{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                            app->y_max() * 5 / 6},
           app->x_max() / 4, app->y_max() / 10, "Back",
           [](Graph_lib::Address, Graph_lib::Address button_addr) {
             App &app = get_app_ref(button_addr);
             app.set_state(new Multiplayer_menu{&app});
           }} {};

void MP_host_menu::enter() {
  app->attach(easy_dif);
  app->attach(medium_dif);
  app->attach(hard_dif);
  app->attach(custom_dif);
  app->attach(area_box);
  app->attach(mines_box);
  app->attach(input_condition);
  input_condition.set_font_size(20);
  app->attach(back);
}

void MP_host_menu::exit() {
  app->detach(easy_dif);
  app->detach(medium_dif);
  app->detach(hard_dif);
  app->detach(custom_dif);
  app->detach(area_box);
  app->detach(mines_box);
  app->detach(input_condition);
  app->detach(back);
}

CellButton::CellButton(Graph_lib::Point xy, Graph_lib::Callback cb, int r,
                       int c, int r_count, int c_count)
    : Button(xy, (500 / r_count), (500 / c_count), "", cb), row{r}, column{c} {}

void CellButton::attach(Graph_lib::Window &win) { Button::attach(win); }

Game::Game(App *app, minesweeper::game_logic::Settings settings)
    : AppState{app}, field{settings},
      quit{Graph_lib::Point{app->x_max() * 8 / 10, app->y_max() * 5 / 6},
           app->x_max() / 8, app->y_max() / 10, "Quit",
           [](Graph_lib::Address, Graph_lib::Address button_addr) {
             App &app = get_app_ref(button_addr);
             app.set_state(new Main_menu(&app));
           }},
      // restart{Graph_lib::Point{app->x_max() * 8 / 10, app->y_max() * 4 / 6},
      //         app->x_max() / 8, app->y_max() / 10, "Restart",
      //         [](Graph_lib::Address, Graph_lib::Address button_addr) {
      //           App &app = get_app_ref(button_addr);
      //           Game &game = dynamic_cast<Game &>(app.get_state());
      //           app.set_state(new Game{&app,
      //           minesweeper::game_logic::Settings{app.}});     DOESN'T WORK
      //         }},
      rec{Graph_lib::Point(350, 150), Graph_lib::Point(750, 450)},
      lose_text{Graph_lib::Point{app->x_max() * 8 / 10, app->y_max() / 5},
                "You lost("},
      win_text{Graph_lib::Point{app->x_max() * 8 / 10, app->y_max() / 5},
               "You win!"}

{
  int size = settings.count_rows < settings.count_columns
                 ? settings.count_rows
                 : settings.count_columns;
  for (int i = 0; i < settings.count_rows; ++i)
    for (int j = 0; j < settings.count_columns; ++j) {
      cells.push_back(new CellButton{
          Graph_lib::Point{300 + j * (500 / size),
                           50 + (settings.count_rows - 1 - i) * (500 / size)},
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            auto widget_p = (static_cast<CellButton *>(button_addr));
            App &app = get_app_ref(button_addr);
            Game &game = dynamic_cast<Game &>(app.get_state());
            Graph_lib::Button &button = get_button_ref(button_addr);
            if (button.is_RMB_click()) {
              game.on_RMB_click(widget_p);
            } else {
              game.on_click(widget_p);
            }
          },
          i, j, settings.count_rows, settings.count_columns});
      marks.push_back(new Graph_lib::Rectangle{
          Graph_lib::Point{300 + j * (500 / size),
                           50 + (settings.count_rows - 1 - i) * (500 / size)},
          Graph_lib::Point{300 + (j + 1) * (500 / size),
                           50 + (settings.count_rows - i) * (500 / size)}});
    }
  // for (int i = 0; i <= settings.count_rows; ++i) {
  //   lines.push_back(
  //       new Graph_lib::Line{Graph_lib::Point{300 + i * (500 / size), 50},
  //                           Graph_lib::Point{300 + i * (500 / size), 545}});
  // }
  // for (int i = 0; i <= settings.count_rows; ++i) {
  //   lines.push_back(
  //       new Graph_lib::Line{Graph_lib::Point{300, 50 + i * (500 / size)},
  //                           Graph_lib::Point{795, 50 + i * (500 / size)}});
  // }
}

void Game::enter() {
  for (int i = 0; i < cells.size(); i++) {
    app->attach(cells[i]);
  }
  for (int i = 0; i < lines.size(); i++) {
    app->attach(lines[i]);
  }
  app->attach(quit);
}

void Game::exit() {
  dettach_all_cells();
  dettach_all_number();
  dettach_all_marks();
  dettach_all_lines();
  app->detach(quit);
  app->detach(lose_text);
  app->detach(win_text);
}

void Game::on_RMB_click(CellButton *btn) {
  minesweeper::game_logic::Field &field = this->field;
  minesweeper::game_logic::IndexPair cell = btn->get_index();

  if (field.get().empty()) {
    field.generate_field(cell);
    attach_number_from_field();
  }

  field.mark_cell(cell);

  this->update();
}

void Game::on_click(CellButton *btn) {
  minesweeper::game_logic::Field &field = this->field;
  minesweeper::game_logic::IndexPair cell = btn->get_index();
  if (field.get().empty()) {
    field.generate_field(cell);
    attach_number_from_field();
  }
  if (!field.is_marked(cell)) {
    field.open_cell(cell);

    this->update();

    if (field.is_bomb(cell)) {
      dettach_all_cells();
      open_all_number();
      attach_lose_state();
    }
    if (field.is_win()) {
      attach_win_state();
    }
  }
}

void Game::update() {
  dettach_all_cells();
  dettach_all_marks();
  attach_all_from_field();
}

void Game::attach_lose_state() {
  // rec.set_fill_color(Graph_lib::Color::red);
  app->attach(quit);
  lose_text.set_font_size(30);
  app->attach(lose_text);
}

void Game::attach_win_state() {
  // rec.set_fill_color(Graph_lib::Color::red);
  app->attach(quit);
  win_text.set_font_size(30);
  app->attach(win_text);
}

void Game::attach_all_from_field() {
  for (int i = 0; i < cells.size(); i++) {

    if (!field.is_open(cells[i].get_index())) {
      if (field.is_marked(cells[i].get_index())) {
        marks[i].set_fill_color(Graph_lib::Color::red);
        app->attach(marks[i]);
      }
      app->attach(cells[i]);
    } else
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
      Numbers[Numbers.size() - 1].set_font_size(20);
      switch (atoi(val.c_str())) {
      case 1:
        Numbers[Numbers.size() - 1].set_color(Graph_lib::Color::blue);
        break;
      case 2:
        Numbers[Numbers.size() - 1].set_color(Graph_lib::Color::dark_blue);
        break;
      case 3:
        Numbers[Numbers.size() - 1].set_color(Graph_lib::Color::green);
        break;
      case 4:
        Numbers[Numbers.size() - 1].set_color(Graph_lib::Color::dark_green);
        break;
      case 5:
        Numbers[Numbers.size() - 1].set_color(Graph_lib::Color::yellow);
        break;
      case 6:
        Numbers[Numbers.size() - 1].set_color(Graph_lib::Color::dark_yellow);
        break;
      case 7:
        Numbers[Numbers.size() - 1].set_color(Graph_lib::Color::red);
        break;

      case 8:
        Numbers[Numbers.size() - 1].set_color(Graph_lib::Color::dark_red);
        break;
      default:
        break;
      }
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
void Game::dettach_all_lines() {
  for (int i = 0; i < lines.size(); i++) {
    app->detach(lines[i]);
  }
}

void Game::dettach_all_cells() {
  for (int i = 0; i < cells.size(); i++) {
    app->detach(cells[i]);
  }
}
void Game::dettach_all_marks() {
  for (int i = 0; i < cells.size(); i++) {
    app->detach(marks[i]);
  }
}
void Game::dettach_all_number() {
  for (int i = 0; i < Numbers.size(); i++) {
    app->detach(Numbers[i]);
  }
}

} // namespace minesweeper::app