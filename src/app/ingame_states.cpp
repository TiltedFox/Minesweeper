#include <app/app_states.h>
#include <app/ingame_states.h>

namespace minesweeper::app {

Multiplayer_ingame::Multiplayer_ingame(App *app, MP_game_type game_type,
                                       Multiplayer_args args)
    : AppState(app), game_type{game_type}, address{args.address},
      input_box({100, 100}, 300, 100, "Sent text"),
      output_box({500, 100}, 300, 100, "Received text"),
      send_button({200, 200}, 100, 50, "Send the text",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    auto &state_ref = dynamic_cast<Multiplayer_ingame &>(
                        get_app_ref(button_addr).get_state());
                    std::string msg = state_ref.input_box.get_string();
                    if (state_ref.ws_p)
                      state_ref.ws_p->async_write(
                          asio::buffer(msg),
                          beast::bind_front_handler(
                              &Multiplayer_ingame::on_write, &state_ref));
                  }),
      resolver{ioc}, acceptor{ioc, tcp::endpoint(tcp::v4(), PORT)},
      settings{args.settings} {
  if (game_type == MP_game_type::host) {
    std::cout << get_local_ip() << " awaiting connection\n";
    acceptor.async_accept(
        ioc, beast::bind_front_handler(&Multiplayer_ingame::on_accept, this));
  } else if (game_type == MP_game_type::client) {
    std::cout << "Tyring to connect!\n";
    ws_p = std::make_unique<websocket::stream<beast::tcp_stream>>(ioc);
    resolver.async_resolve(
        address, std::to_string(PORT),
        beast::bind_front_handler(&Multiplayer_ingame::on_resolve, this));
  }
  thread = std::thread{[&ioc = ioc]() { ioc.run(); }};
}

std::string Multiplayer_ingame::get_local_ip() {
  tcp::resolver::results_type endpoints =
      resolver.resolve(asio::ip::host_name(), "");
  for (auto endpoint : endpoints) {
    std::string ip_addr = endpoint.endpoint().address().to_string();
    if (ip_addr.substr(0, 3) == "192")
      return ip_addr;
  }
  return "";
}

void Multiplayer_ingame::on_accept(beast::error_code ec, tcp::socket socket) {
  if (ec)
    std::cerr << ec.message() << "\n";

  std::cout << "Client tcp connection successful\n";

  ws_p =
      std::make_unique<websocket::stream<beast::tcp_stream>>(std::move(socket));
  // accept handshake
  ws_p->async_accept(
      beast::bind_front_handler(&Multiplayer_ingame::on_handshake, this));
}

void Multiplayer_ingame::on_handshake(beast::error_code ec) {
  if (ec)
    std::cerr << ec.message() << "\n";
  std::cout << "Successful websocket handshake!\n";
  ws_p->async_read(read_buffer, beast::bind_front_handler(
                                    &Multiplayer_ingame::on_read, this));
}

void Multiplayer_ingame::on_read(beast::error_code ec,
                                 std::size_t bytes_transferred) {
  unsigned char *data = static_cast<unsigned char *>(read_buffer.data().data());
  std::string msg;
  for (int i = 0; i < read_buffer.data().size(); ++i)
    msg += data[i];
  output_box.put(msg);
  read_buffer.clear();
  ws_p->async_read(read_buffer, beast::bind_front_handler(
                                    &Multiplayer_ingame::on_read, this));
}

void Multiplayer_ingame::on_resolve(beast::error_code ec,
                                    tcp::resolver::results_type results) {

  std::cout << "Connection resolved!\n";
  if (ec)
    std::cerr << ec.message() << "\n";

  beast::get_lowest_layer(*ws_p.get())
      .async_connect(results, beast::bind_front_handler(
                                  &Multiplayer_ingame::on_connect, this));
}

void Multiplayer_ingame::on_connect(
    beast::error_code ec, tcp::resolver::results_type::endpoint_type ep) {
  if (ec)
    std::cerr << ec.message() << "\n";
  std::cout << "Connected!\n";

  // Turn off the timeout on the tcp_stream, because
  // the websocket stream has its own timeout system.
  beast::get_lowest_layer(*ws_p.get()).expires_never();

  ws_p->async_handshake(
      address + ":" + std::to_string(ep.port()), "/",
      beast::bind_front_handler(&Multiplayer_ingame::on_handshake, this));
}

void Multiplayer_ingame::on_write(beast::error_code ec,
                                  std::size_t bytes_transferred) {
  if (ec)
    std::cerr << ec.message() << "\n";
  std::cout << "Wrote " << bytes_transferred << "bytes\n";
}

void Multiplayer_ingame::enter() {
  app->attach(input_box);
  app->attach(output_box);
  app->attach(send_button);
}

void Multiplayer_ingame::exit() {
  app->detach(input_box);
  app->detach(output_box);
  app->detach(send_button);
}

} // namespace minesweeper::app