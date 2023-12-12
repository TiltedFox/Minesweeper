#ifndef INGAME_STATES_H
#define INGAME_STATES_H

#include <app/app.h>

namespace minesweeper::app {

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
  // boost::thread thread;
  std::thread thread;
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
  // boost::thread thread;
  std::thread thread;
  asio::executor_work_guard<asio::io_context::executor_type> work_guard =
      asio::make_work_guard(ioc);
};

} // namespace minesweeper::app

#endif // ifndef INGAME_STATES_H