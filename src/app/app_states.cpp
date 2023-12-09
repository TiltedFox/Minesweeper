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
                 app.set_state(new Game{&app});
               }},
      medium_dif{Graph_lib::Point{app->x_max() / 5, app->y_max() / 2},
                 app->x_max() / 5, app->y_max() / 10, "Medium",
                 [](Graph_lib::Address, Graph_lib::Address button_addr) {
                   App &app = get_app_ref(button_addr);
                   app.set_state(new Game{&app});
                 }},
      hard_dif{Graph_lib::Point{app->x_max() / 5, app->y_max() * 2 / 3},
               app->x_max() / 5, app->y_max() / 10, "Hard",
               [](Graph_lib::Address, Graph_lib::Address button_addr) {
                 App &app = get_app_ref(button_addr);
                 app.set_state(new Game{&app});
               }},
      custom_dif{Graph_lib::Point{app->x_max() * 3 / 5, app->y_max() / 3},
                 app->x_max() / 5, app->y_max() / 10, "Custom",
                 [](Graph_lib::Address, Graph_lib::Address button_addr) {
                   App &app = get_app_ref(button_addr);
                   app.set_state(new Game{&app});
                 }},
      area_box{Graph_lib::Point{app->x_max() * 3 / 5, app->y_max() / 2},
               app->x_max() / 5, app->y_max() / 10, "Area"},
      mines_box{Graph_lib::Point{app->x_max() * 3 / 5, app->y_max() * 2 / 3},
                app->x_max() / 5, app->y_max() / 10, "Mines"},
      start_game_button{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                                         app->y_max() * 5 / 6},
                        app->x_max() / 4, app->y_max() / 10, "Start game",
                        [](Graph_lib::Address, Graph_lib::Address button_addr) {
                          App &app = get_app_ref(button_addr);
                          app.set_state(new Game{&app});
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
      start_game_button{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                                         app->y_max() * 5 / 6},
                        app->x_max() / 4, app->y_max() / 10, "Start game",
                        [](Graph_lib::Address, Graph_lib::Address button_addr) {
                          App &app = get_app_ref(button_addr);
                          app.set_state(new Game{&app});
                        }} {};

void Multiplayer_menu::enter() {
  app->attach(ip_box);
  app->attach(start_game_button);
}

void Multiplayer_menu::exit() {
  app->detach(ip_box);
  app->detach(start_game_button);
}

Button_Cell::Button_Cell(Graph_lib::Point xy, Graph_lib::Callback cb, int r, int c)
  :Button(xy, size, size, "", cb)
  , row{r}
  , column{c} {}

void Button_Cell::attach(Graph_lib::Window& win) {
  Button::attach(win);
}

int r = 8;
int c = 8;

Game::Game(App *app) 
    : AppState{app}
    ,field{nullptr}
    {
      for (int i = 0; i < r; ++i)
      for (int j = 0; j < c; ++j)
      {
          cells.push_back (new Button_Cell{Graph_lib::Point{20 + j*Button_Cell::size,
                                          20 + (r -1 - i)*Button_Cell::size},
                                    [](Graph_lib::Address, Graph_lib::Address button_addr) {
                                    auto widget_p = (static_cast<Button_Cell *>(button_addr));
                                    clicked(widget_p);
                                    }
                                    , i, j});
      }
      r ++;
      c ++;
    }
void Game::enter() {
  for (int i = 0; i < cells.size(); i++) {
    app -> attach(cells[i]) ;
  }
}
void Game::exit() {
  for (int i = 0; i < cells.size(); i++) {
    app -> detach(cells[i]);
  }
}

void Game::clicked(Button_Cell* c) {
  minesweeper::game_logic::IndexPair pair = c->get_user_click(); // pair = { row : int , column : int }
  c->hide();
  //функция которая получит новое состояние матрицы поля
  //функция которая перерисует
  std::cout <<"pair" << pair.row  << " " << pair.column << std::endl;

}
} // namespace minesweeper::app