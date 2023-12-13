#ifndef INGAME_STATES_H
#define INGAME_STATES_H

#include <app/app.h>

namespace minesweeper::app {

struct Field_widget;

struct Cell_button : public Graph_lib::Button {
  Cell_button(Field_widget *field_widget, Graph_lib::Point xy, int w, int h,
              int row, int column, Graph_lib::Callback cb)
      : Graph_lib::Button(xy, w, h, "", cb), field_widget{field_widget},
        row{row}, column{column} {}

  game_logic::IndexPair indexes() { return {row, column}; }

  static void callback(Graph_lib::Address, Graph_lib::Address cell_addr);

  Field_widget *field_widget;
  int row;
  int column;
};

enum class Field_result { won, lost };
enum class Cell_interaction { open, mark };
using CLR = Graph_lib::Color;
static std::map<std::string, CLR> CELL_COLORS{
    {"0", CLR(CLR::invisible)},   {"1", CLR(CLR::blue)},
    {"2", CLR(CLR::dark_blue)},   {"3", CLR(CLR::green)},
    {"4", CLR(CLR::dark_green)},  {"5", CLR(CLR::yellow)},
    {"6", CLR(CLR::dark_yellow)}, {"7", CLR(CLR::red)},
    {"8", CLR(CLR::dark_red)},    {"9", CLR(CLR::black)}};

struct Field_widget {
  Field_widget(App *app, game_logic::Settings settings, Graph_lib::Point xy,
               int w, int h, Graph_lib::Callback cell_callback,
               std::function<void(Field_result)> end_callback);

  void attach();
  void detach();

  void cell_interact(game_logic::IndexPair indexes,
                     Cell_interaction interactio);

  void update();
  void end_game(Field_result res);

  App *app;
  game_logic::Settings settings;
  Graph_lib::Point upper_left;
  int width;
  int height;

  game_logic::Field field;
  Graph_lib::Vector_ref<Cell_button> cells;
  Graph_lib::Vector_ref<Graph_lib::Rectangle> marks;
  Graph_lib::Vector_ref<Graph_lib::Text> numbers;
  Graph_lib::Lines borders;

  std::function<void(Field_result)> end_callback;
};

class Test : public AppState {
public:
  Test(App *app, minesweeper::game_logic::Settings settings)
      : AppState(app),
        field1(app, settings, {100, 50}, 500, 500, Cell_button::callback,
               std::bind(&Test::game_ended, this, std::placeholders::_1)){};

  void enter() override { field1.attach(); };
  void exit() override { field1.detach(); };

  void game_ended(Field_result res) {
    if (res == Field_result::won)
      std::cout << "WIN\n";

    else
      std::cout << "DEFEAT!\n";
  }

  Field_widget field1;
};

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
  game_logic::Settings settings{0, 0, 0};

  Multiplayer_args(std::string addr) : address{addr} {};
  Multiplayer_args(game_logic::Settings settings) : settings{settings} {};
};

class Multiplayer_ingame : public AppState {
public:
  Multiplayer_ingame(App *app, MP_game_type game_type, Multiplayer_args args);

  void enter() override;
  void exit() override;

  MP_game_type game_type;

  game_logic::Settings settings;

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