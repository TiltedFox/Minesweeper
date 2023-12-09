#include <app/app_states.h>

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
                 app.set_state(new Game_menu{&app});
               }},
      medium_dif{Graph_lib::Point{app->x_max() / 5, app->y_max() / 2},
                 app->x_max() / 5, app->y_max() / 10, "Medium",
                 [](Graph_lib::Address, Graph_lib::Address button_addr) {
                   App &app = get_app_ref(button_addr);
                   app.set_state(new Game_menu{&app});
                 }},
      hard_dif{Graph_lib::Point{app->x_max() / 5, app->y_max() * 2 / 3},
               app->x_max() / 5, app->y_max() / 10, "Hard",
               [](Graph_lib::Address, Graph_lib::Address button_addr) {
                 App &app = get_app_ref(button_addr);
                 app.set_state(new Game_menu{&app});
               }},
      custom_dif{Graph_lib::Point{app->x_max() * 3 / 5, app->y_max() / 3},
                 app->x_max() / 5, app->y_max() / 10, "Custom",
                 [](Graph_lib::Address, Graph_lib::Address button_addr) {
                   App &app = get_app_ref(button_addr);
                   app.set_state(new Game_menu{&app});
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
                          app.set_state(new Game_menu{&app});
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
                          app.set_state(new Game_menu{&app});
                        }} {};

void Multiplayer_menu::enter() {
  app->attach(ip_box);
  app->attach(start_game_button);
}

void Multiplayer_menu::exit() {
  app->detach(ip_box);
  app->detach(start_game_button);
}

Game_menu::Game_menu(App *app)
    : AppState{app},
      test_text{Graph_lib::Point{(app->x_max() - app->x_max() / 4) / 2,
                                 app->y_max() / 2},
                "Game menu!"} {};

void Game_menu::enter() { app->attach(test_text); }

void Game_menu::exit() { app->detach(test_text); }

Test::Test(App *app)
    : AppState(app),
      test_button({50, 50}, 50, 50, "LOL",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    Graph_lib::Button &button = get_button_ref(button_addr);
                    if (button.is_LMB_click())
                      std::cout << "Left click";
                    else if (button.is_RMB_click())
                      std::cout << "Right click";
                  }){};

} // namespace minesweeper::app