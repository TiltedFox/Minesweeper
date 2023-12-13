#include <app/ingame_states.h>
#include <app/menu_states.h>

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
                 app.set_state(new Test{&app, minesweeper::game_logic::kEasy});
               }},
      medium_dif{
          Graph_lib::Point{app->x_max() * 3 / 7, app->y_max() / 3},
          app->x_max() / 7, app->y_max() / 10, "Medium",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            app.set_state(new Test{&app, minesweeper::game_logic::kMedium});
          }},
      hard_dif{Graph_lib::Point{app->x_max() * 9 / 14, app->y_max() / 3},
               app->x_max() / 7, app->y_max() / 10, "Hard",
               [](Graph_lib::Address, Graph_lib::Address button_addr) {
                 App &app = get_app_ref(button_addr);
                 app.set_state(new Test{&app, minesweeper::game_logic::kHard});
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
              app.set_state(new Test{&app, minesweeper::game_logic::Settings{
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
            app.set_state(new Test{&app, minesweeper::game_logic::kEasy});
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
                 app.set_state(new Test{&app, minesweeper::game_logic::kEasy});
               }},
      medium_dif{
          Graph_lib::Point{app->x_max() * 3 / 7, app->y_max() / 3},
          app->x_max() / 7, app->y_max() / 10, "Medium",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            App &app = get_app_ref(button_addr);
            app.set_state(new Test{&app, minesweeper::game_logic::kMedium});
          }},
      hard_dif{Graph_lib::Point{app->x_max() * 9 / 14, app->y_max() / 3},
               app->x_max() / 7, app->y_max() / 10, "Hard",
               [](Graph_lib::Address, Graph_lib::Address button_addr) {
                 App &app = get_app_ref(button_addr);
                 app.set_state(new Test{&app, minesweeper::game_logic::kHard});
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
              app.set_state(new Test{&app, minesweeper::game_logic::Settings{
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

} // namespace minesweeper::app