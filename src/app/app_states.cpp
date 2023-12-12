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
    : AppState(app),
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
                  }),
      mp_client_button({200, 200}, 100, 50, "Client",
                       [](Graph_lib::Address, Graph_lib::Address button_addr) {
                         App &app = get_app_ref(button_addr);
                         app.set_state(new Multiplayer_game_client{&app});
                       }),
      mp_server_button({400, 200}, 100, 50, "Server",
                       [](Graph_lib::Address, Graph_lib::Address button_addr) {
                         App &app = get_app_ref(button_addr);
                         app.set_state(new Multiplayer_game_server{&app});
                       }){};

Multiplayer_game_server::Multiplayer_game_server(App *app)
    : AppState(app),
      test_button({400, 200}, 100, 50, "Test",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    std::cout << "Working";
                  }) {
  std::cout << "Awaiting connection";
  acceptor.async_accept(ioc, beast::bind_front_handler(
                                 &Multiplayer_game_server::on_accept, this));
  thread = boost::thread(boost::bind(&boost::asio::io_context::run, &ioc));
}

void Multiplayer_game_server::on_accept(beast::error_code ec,
                                        tcp::socket socket) {
  if (ec)
    std::cerr << ec.message() << "\n";
  std::cout << "Client tcp connection successful\n";
  ws_p =
      std::make_unique<websocket::stream<beast::tcp_stream>>(std::move(socket));

  // accept handshake
  ws_p->async_accept(
      beast::bind_front_handler(&Multiplayer_game_server::on_handshake, this));
}

void Multiplayer_game_server::on_handshake(beast::error_code ec) {
  if (ec)
    std::cerr << ec.message() << "\n";
  std::cout << "Successful websocket handshake!\n";
  ws_p->async_read(buffer, beast::bind_front_handler(
                               &Multiplayer_game_server::on_read, this));
}

void Multiplayer_game_server::on_read(beast::error_code ec,
                                      std::size_t bytes_transferred) {
  unsigned char *data = static_cast<unsigned char *>(buffer.data().data());
  for (int i = 0; i < buffer.data().size(); ++i)
    std::cout << data[i];
  std::cout << "\n";
  buffer.clear();
  ws_p->async_read(buffer, beast::bind_front_handler(
                               &Multiplayer_game_server::on_read, this));
}

Multiplayer_game_client::Multiplayer_game_client(App *app)
    : AppState(app),
      test_button({400, 200}, 100, 50, "Test",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    std::cout << "Working";
                  }),
      input_box({100, 100}, 300, 100, "Some text"),
      send_button({200, 200}, 100, 50, "Send the text",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    auto &state_ref = dynamic_cast<Multiplayer_game_client &>(
                        get_app_ref(button_addr).get_state());
                    std::string msg = state_ref.input_box.get_string();
                    state_ref.ws.async_write(
                        asio::buffer(msg),
                        beast::bind_front_handler(
                            &Multiplayer_game_client::on_write, &state_ref));
                  }) {
  resolver.async_resolve(
      tcp::endpoint(address, 25566),
      beast::bind_front_handler(&Multiplayer_game_client::on_resolve, this));
  thread = boost::thread(boost::bind(&boost::asio::io_context::run, &ioc));
}

void Multiplayer_game_client::on_resolve(beast::error_code ec,
                                         tcp::resolver::results_type results) {
  std::cout << "Connection resolved!\n";
  if (ec)
    std::cerr << ec.message() << "\n";

  beast::get_lowest_layer(ws).async_connect(
      results,
      beast::bind_front_handler(&Multiplayer_game_client::on_connect, this));
}

void Multiplayer_game_client::on_connect(
    beast::error_code ec, tcp::resolver::results_type::endpoint_type ep) {
  if (ec)
    std::cerr << ec.message() << "\n";
  std::cout << "Connected!\n";

  // Turn off the timeout on the tcp_stream, because
  // the websocket stream has its own timeout system.
  beast::get_lowest_layer(ws).expires_never();

  ws.async_handshake(
      "127.0.0.1" + ep.port(), "/",
      beast::bind_front_handler(&Multiplayer_game_client::on_handshake, this));
}

void Multiplayer_game_client::on_handshake(beast::error_code ec) {
  if (ec)
    std::cerr << ec.message() << "\n";
  std::cout << "Successful handshake!\n";
  ws.async_write(
      asio::buffer("Hello from client!"),
      beast::bind_front_handler(&Multiplayer_game_client::on_write, this));
}

void Multiplayer_game_client::on_write(beast::error_code ec,
                                       std::size_t bytes_transferred) {
  if (ec)
    std::cerr << ec.message() << "\n";
  std::cout << "Wrote " << bytes_transferred << "bytes";
}

} // namespace minesweeper::app