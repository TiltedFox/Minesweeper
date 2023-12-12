#ifndef APP_STATES_H
#define APP_STATES_H

#include <app/app.h>

namespace minesweeper::app {

class Main_menu;
class Singleplayer_menu;
class Multiplayer_menu;
class Game_menu;

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

class Game_menu : public AppState {
public:
  Game_menu(App *app);

  void enter() override;
  void exit() override;

private:
  Graph_lib::Text test_text;
};

class Test : public AppState {
public:
  Test(App *app);

  void enter() override {
    app->attach(test_button);
    app->attach(mp_client_button);
    app->attach(mp_server_button);
  };
  void exit() override {
    app->detach(test_button);
    app->detach(mp_client_button);
    app->detach(mp_server_button);
  }

  ;

private:
  Graph_lib::Button test_button;
  Graph_lib::Button mp_server_button;
  Graph_lib::Button mp_client_button;
};

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace asio = boost::asio;           // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;

class Multiplayer_game_server : public AppState {
public:
  Multiplayer_game_server(App *app);

  void enter() override { app->attach(test_button); };
  void exit() override { app->detach(test_button); };

  Graph_lib::Button test_button;

  void on_accept(beast::error_code ec, tcp::socket socket);
  void on_handshake(beast::error_code ec);
  void on_read(beast::error_code ec, std::size_t bytes_transferred);

  // void timer_print() {
  //   std::cout << "Timer ticked\n";
  //   t.expires_at(t.expiry() + asio::chrono::seconds(5));
  //   t.async_wait(boost::bind(&Multiplayer_game_server::timer_print, this));
  // }
  // asio::steady_timer t{ioc, asio::chrono::seconds(5)};

  asio::io_context ioc;
  asio::ip::address address = asio::ip::make_address("127.0.0.1");
  tcp::acceptor acceptor = tcp::acceptor(ioc, tcp::endpoint(address, 25566));
  std::unique_ptr<websocket::stream<beast::tcp_stream>> ws_p;
  boost::thread thread;
  beast::flat_buffer buffer;
};

class Multiplayer_game_client : public AppState {
public:
  Multiplayer_game_client(App *app);

  void enter() override {
    app->attach(test_button);
    app->attach(input_box);
    app->attach(send_button);
  };
  void exit() override {
    app->detach(test_button);
    app->detach(input_box);
    app->detach(send_button);
  };

  Graph_lib::Button test_button;
  Graph_lib::In_box input_box;
  Graph_lib::Button send_button;

  void on_resolve(beast::error_code ec, tcp::resolver::results_type results);
  void on_connect(beast::error_code ec,
                  tcp::resolver::results_type::endpoint_type ep);
  void on_handshake(beast::error_code ec);
  void on_write(beast::error_code ec, std::size_t bytes_transferred);

  // void timer_print() {
  //   std::cout << "Timer ticked\n";
  //   t.expires_at(t.expiry() + asio::chrono::seconds(5));
  //   t.async_wait(boost::bind(&Multiplayer_game_server::timer_print,
  //   this));
  // }
  // asio::steady_timer t{ioc, asio::chrono::seconds(5)};

  asio::io_context ioc;
  asio::ip::address address = asio::ip::make_address("127.0.0.1");
  tcp::resolver resolver{ioc};
  // tcp::socket socket = tcp::socket(ioc);
  websocket::stream<beast::tcp_stream> ws{ioc};
  boost::thread thread;
  asio::executor_work_guard<asio::io_context::executor_type> work_guard =
      asio::make_work_guard(ioc);
};

} // namespace minesweeper::app

#endif // #ifndef APP_STATES_H