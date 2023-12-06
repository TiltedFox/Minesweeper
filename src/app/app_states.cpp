#include <app/app_states.h>

namespace minesweeper::app {
<<<<<<< HEAD
Singleplayer_menu::Singleplayer_menu()
    : easy_dif{
          Graph_lib::Point{330, 300}, 70, 20, "Easy",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            get_app_ref(button_addr).set_state(Game_menu::get_instance());
          }},
      medium_dif{
          Graph_lib::Point{330, 300}, 70, 20, "Medium",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            get_app_ref(button_addr).set_state(Game_menu::get_instance());
          }},
      hard_dif{
          Graph_lib::Point{330, 300}, 70, 20, "Hard",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            get_app_ref(button_addr).set_state(Game_menu::get_instance());
          }},
      custom_dif{
          Graph_lib::Point{330, 300}, 70, 20, "Easy",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            get_app_ref(button_addr).set_state(Game_menu::get_instance());
          }},
      area_box{
          Graph_lib::Point{330, 300}, 70, 20, "Easy"
          },
      mines_box{
          Graph_lib::Point{330, 300}, 70, 20, "Easy"
          },
      start_game_button{
          Graph_lib::Point{465, 530}, 70, 20, "Start game",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            get_app_ref(button_addr).set_state(Game_menu::get_instance());
          }} {};


Multiplayer_menu::Multiplayer_menu()
  : ip_box{Graph_lib::Point{465, 500}, 70, 20, "Ip-adress"},
    start_game_button{
          Graph_lib::Point{465, 530}, 70, 20, "Start game",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            get_app_ref(button_addr).set_state(Game_menu::get_instance());
          }} {};


Main_menu::Main_menu()
    : singleplayer_button{
          Graph_lib::Point{465, 350}, 70, 20, "Singleplayer",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            get_app_ref(button_addr).set_state(Singleplayer_menu::get_instance());
          }},
      multiplayer_button{
          Graph_lib::Point{465, 380}, 70, 20, "Multiplayer",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            get_app_ref(button_addr).set_state(Multiplayer_menu::get_instance());
          }},
      quit{
          Graph_lib::Point{465, 410}, 70, 20, "Quit",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            get_app_ref(button_addr).set_state(Main_menu::get_instance());
          }},
      header{
          Graph_lib::Point{465, 350}, "Minesweeper",
          } {};

void Main_menu::enter(App *app) {
  app->attach(singleplayer_button);
  app->attach(multiplayer_button);
  app->attach(quit);
}

void Main_menu::exit(App *app) {
  app->detach(singleplayer_button);
  app->detach(multiplayer_button);
  app->detach(quit);
}

Game_menu::Game_menu() {};
=======
TestState2::TestState2(App *app)
    : AppState{app},
      test_button{Graph_lib::Point{100, 100}, 50, 50, "Change to state1",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    App &app = get_app_ref(button_addr);
                    app.set_state(new TestState1{&app});
                  }} {};

TestState1::TestState1(App *app)
    : AppState{app},
      test_button{Graph_lib::Point{20, 20}, 50, 50, "Change to state2",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    App &app = get_app_ref(button_addr);
                    app.set_state(new TestState2{&app});
                  }} {};
>>>>>>> dev
} // namespace minesweeper::app