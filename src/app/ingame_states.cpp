#include <app/app_states.h>
#include <app/ingame_states.h>

namespace minesweeper::app {

void Cell_button::callback(Graph_lib::Address, Graph_lib::Address cell_addr) {
  Cell_button &cell = *static_cast<Cell_button *>(cell_addr);
  Field_widget &field_widget = *cell.field_widget;

  if (cell.is_RMB_click())
    field_widget.cell_interact(cell.indexes(), Cell_interaction::mark);
  else if (cell.is_LMB_click())
    field_widget.cell_interact(cell.indexes(), Cell_interaction::open);
}

Field_widget::Field_widget(App *app, game_logic::Settings settings,
                           Graph_lib::Point xy, int w, int h,
                           Graph_lib::Callback cell_callback,
                           std::function<void(Field_result)> end_callback)
    : app{app}, settings{settings}, upper_left{xy}, width{w}, height{h},
      field{settings}, end_callback{end_callback} {
  int widget_width = width / settings.count_columns;
  int widget_height = height / settings.count_rows;

  int calc_width = widget_width * settings.count_columns;
  int calc_height = widget_height * settings.count_rows;
  borders.add(upper_left, {upper_left.x + calc_width, upper_left.y});
  borders.add(upper_left, {upper_left.x, upper_left.y + calc_height});
  borders.add({upper_left.x + calc_width, upper_left.y},
              {upper_left.x + calc_width, upper_left.y + calc_height});
  borders.add({upper_left.x, upper_left.y + calc_height},
              {upper_left.x + calc_width, upper_left.y + calc_height});

  for (int row = 0; row < settings.count_rows; ++row)
    for (int column = 0; column < settings.count_columns; ++column) {
      Graph_lib::Point current_upper_left{upper_left.x + widget_width * column,
                                          upper_left.y + widget_height * row};
      cells.push_back(new Cell_button(this, current_upper_left, widget_width,
                                      widget_height, row, column,
                                      cell_callback));
      marks.push_back(new Graph_lib::Rectangle(current_upper_left, widget_width,
                                               widget_height));
      marks[marks.size() - 1].set_color(Graph_lib::Color::invisible);
      marks[marks.size() - 1].set_fill_color(Graph_lib::Color::invisible);
      numbers.push_back(
          new Graph_lib::Text({current_upper_left.x + widget_width / 4,
                               current_upper_left.y + widget_height * 3 / 4},
                              "")); // point is bottom left of first letter
      numbers[numbers.size() - 1].set_font_size(
          std::min(widget_width * 2 / 3, widget_height * 2 / 3));
    }
}

void Field_widget::attach() {
  for (int i = 0; i < settings.count_columns * settings.count_rows; ++i) {
    app->attach(cells[i]);
    app->attach(marks[i]);
    app->attach(numbers[i]);
  }
  app->attach(borders);
}

void Field_widget::detach() {
  for (int i = 0; i < settings.count_columns * settings.count_rows; ++i) {
    app->detach(cells[i]);
    app->detach(marks[i]);
    app->detach(numbers[i]);
  }
  app->detach(borders);
}

void Field_widget::cell_interact(game_logic::IndexPair indexes,
                                 Cell_interaction interaction) {
  if (field.get().empty())
    field.generate_field(indexes);

  switch (interaction) {
  case Cell_interaction::mark:
    field.mark_cell(indexes);
    break;
  case Cell_interaction::open:
    if (field.is_marked(indexes))
      return;
    if (field.is_bomb(indexes))
      return end_game(Field_result::lost);
    field.open_cell(indexes);
    if (field.is_win())
      return end_game(Field_result::won);
    break;
  }

  update();
}

void Field_widget::update() {
  for (int i = 0; i < settings.count_columns * settings.count_rows; ++i) {
    game_logic::IndexPair cell = cells[i].indexes();
    if (field.is_open(cell)) {
      marks[i].set_fill_color(Graph_lib::Color::invisible);
      cells[i].hide();

      std::string label =
          std::to_string(field.get()[cell.row][cell.column].count_bomb);
      numbers[i].set_color(CELL_COLORS.at(label));
      if (label == "9") // 9 stands for bomb
        label = "*";
      else if (label == "0")
        label = "";
      numbers[i].set_label(label);
    } else {
      if (field.is_marked(cell))
        marks[i].set_fill_color(Graph_lib::Color::red);
      else
        marks[i].set_fill_color(Graph_lib::Color::invisible);
      cells[i].show();
    }
  }
}

void Field_widget::end_game(Field_result res) {
  for (int i = 0; i < settings.count_columns * settings.count_rows; ++i)
    field.open_cell(cells[i].indexes());
  update();
  end_callback(res);
}

Multiplayer_ingame::Multiplayer_ingame(App *app, MP_game_type game_type,
                                       Multiplayer_args args)
    : AppState(app), game_type{game_type}, settings{args.settings},
      address{args.address}, resolver{ioc},
      acceptor{ioc, tcp::endpoint(tcp::v4(), PORT)},
      input_box({100, 100}, 300, 100, "Sent text"),
      output_box({500, 100}, 300, 100, "Received text"),
      send_button(
          {200, 200}, 100, 50, "Send the text",
          [](Graph_lib::Address, Graph_lib::Address button_addr) {
            auto &state_ref = get_state_ref<Multiplayer_ingame>(button_addr);
            state_ref.write_json({{"message", state_ref.input_box.get_string()},
                                  {"test", "123"}});
          }) {
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
  std::cout << "Wrote " << bytes_transferred << " bytes\n";
}

void Multiplayer_ingame::write_json(json::value val) {
  if (!ws_p)
    std::cerr << "Attempted write with no socket open\n";
  ws_p->async_write(
      asio::buffer(json::serialize(val)),
      beast::bind_front_handler(&Multiplayer_ingame::on_write, this));
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