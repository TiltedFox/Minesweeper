#ifndef INGAME_STATES_H
#define INGAME_STATES_H

#include <app/app.h>

namespace minesweeper::app {

namespace json = boost::json;           // from <boost/json.hpp>
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace asio = boost::asio;           // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;

const unsigned int PORT = 25566;

enum class MP_game_type { host, client };

struct Multiplayer_args {
  std::string address = "";
  minesweeper::game_logic::Settings settings{0, 0, 0};

  Multiplayer_args(std::string addr) : address{addr} {};
  Multiplayer_args(minesweeper::game_logic::Settings settings)
      : settings{settings} {};
};

class Multiplayer_ingame : public AppState {
public:
  Multiplayer_ingame(App *app, MP_game_type game_type, Multiplayer_args args);

  void enter() override;
  void exit() override;

  MP_game_type game_type;

  minesweeper::game_logic::Settings settings;

  // server
  std::string get_local_ip();
  void on_accept(beast::error_code ec, tcp::socket socket);
  // client
  void on_resolve(beast::error_code ec, tcp::resolver::results_type results);
  void on_connect(beast::error_code ec,
                  tcp::resolver::results_type::endpoint_type ep);
  // common
  void on_handshake(beast::error_code ec);
  void on_write(beast::error_code ec, std::size_t bytes_transferred);
  void on_read(beast::error_code ec, std::size_t bytes_transferred);

  void write_json(json::value val);

  asio::io_context ioc;
  std::string address;
  tcp::resolver resolver;
  tcp::acceptor acceptor;
  std::unique_ptr<websocket::stream<beast::tcp_stream>> ws_p;
  std::thread thread;
  beast::flat_buffer read_buffer;

  Graph_lib::In_box input_box;
  Graph_lib::Out_box output_box;
  Graph_lib::Button send_button;
};

} // namespace minesweeper::app

#endif // ifndef INGAME_STATES_H