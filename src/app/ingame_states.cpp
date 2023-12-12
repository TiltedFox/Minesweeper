#include <app/app_states.h>
#include <app/ingame_states.h>

namespace minesweeper::app {

Multiplayer_game_server::Multiplayer_game_server(App *app)
    : AppState(app),
      test_button({400, 200}, 100, 50, "Test",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    std::cout << "Working";
                  }) {
  std::cout << "Awaiting connection";
  acceptor.async_accept(ioc, beast::bind_front_handler(
                                 &Multiplayer_game_server::on_accept, this));
  thread = std::thread{[&ioc = ioc]() { ioc.run(); }};
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
  thread = std::thread{[&ioc = ioc]() { ioc.run(); }};
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