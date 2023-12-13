#include <app/ingame_states.h>
#include <app/menu_states.h>

namespace minesweeper::app {

void Cell_button::callback(Graph_lib::Address, Graph_lib::Address cell_addr) {
  Cell_button &cell = *static_cast<Cell_button *>(cell_addr);
  Field_widget &field_widget = *cell.field_widget;

  if (cell.is_RMB_click())
    field_widget.cell_interact(cell.indexes(), Cell_interaction::mark);
  else if (cell.is_LMB_click())
    field_widget.cell_interact(cell.indexes(), Cell_interaction::open);
}

void Cell_button::callback_multiplayer(Graph_lib::Address,
                                       Graph_lib::Address cell_addr) {
  Multiplayer_ingame &state = get_state_ref<Multiplayer_ingame>(cell_addr);
  Cell_button &cell = *static_cast<Cell_button *>(cell_addr);

  std::string interaction;

  if (cell.is_RMB_click())
    interaction = "mark";
  else if (cell.is_LMB_click())
    interaction = "open";

  json::value move_val{
      {"type", "move"},
      {"cell_indexes", {cell.indexes().row, cell.indexes().column}},
      {"interaction", interaction}};

  state.write_json(move_val);

  Cell_button::callback(nullptr, cell_addr);
}

Field_widget::Field_widget(App *app, game_logic::Settings settings,
                           Graph_lib::Point xy, int w, int h,
                           Graph_lib::Callback cell_callback,
                           std::function<void(Field_result)> end_callback)
    : app{app}, settings{settings}, upper_left{xy}, width{w}, height{h},
      field{settings}, end_callback{end_callback} {
  init_field(cell_callback);
}

Field_widget::Field_widget(App *app, const game_logic::Field &field,
                           Graph_lib::Point xy, int w, int h,
                           Graph_lib::Callback cell_callback,
                           std::function<void(Field_result)> end_callback)
    : app{app}, settings{field.get_settings()}, upper_left{xy}, width{w},
      height{h}, field{field}, end_callback{end_callback} {
  init_field(cell_callback);
}

void Field_widget::init_field(Graph_lib::Callback cell_callback) {
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
      output_box({app->x_max() / 2 - 150, 10}, 300, 30, ""),
      quit_button({app->x_max() / 2 - 50, app->y_max() - 40}, 100, 30, "Quit",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    App &app = get_app_ref(button_addr);
                    Multiplayer_ingame &state =
                        get_state_ref<Multiplayer_ingame>(button_addr);

                    state.close_connection();
                    state.ioc.stop();
                    state.thread.join();
                    app.set_state(new Main_menu(&app));
                  }) {
  if (game_type == MP_game_type::host) {
    // std::cout << "Awating";
    acceptor.async_accept(
        ioc, beast::bind_front_handler(&Multiplayer_ingame::on_accept, this));
  } else if (game_type == MP_game_type::client) {
    std::cout << "Trying to connect!\n";
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

  write_output("Connected!");

  if (game_type == MP_game_type::host) {
    game_logic::IndexPair start_pos = server_create_fields();
    std::vector<std::pair<int, int>> bombs;
    const game_logic::field_matrix_t &cells = own_field->field.get();

    for (int i = 0; i < settings.count_rows; ++i)
      for (int j = 0; j < settings.count_columns; ++j)
        if (cells[i][j].count_bomb == 9)
          bombs.push_back({i, j});

    json::value field_val = {
        {"type", "field"},
        {"settings",
         {settings.count_rows, settings.count_columns, settings.count_bomb}},
        {"bombs", json::value_from(bombs)},
        {"start_pos", {start_pos.row, start_pos.column}}};
    write_json(field_val);
  }

  ws_p->async_read(read_buffer, beast::bind_front_handler(
                                    &Multiplayer_ingame::on_read, this));
}

void Multiplayer_ingame::on_read(beast::error_code ec, std::size_t) {
  if (ec) {
    return clean_up(ec);
  }

  unsigned char *data = static_cast<unsigned char *>(read_buffer.data().data());
  std::string raw;
  for (std::size_t i = 0; i < read_buffer.data().size(); ++i)
    raw += data[i];
  read_buffer.clear();
  json::value msg = json::parse(raw);
  if (game_type == MP_game_type::client) {
    if (msg.at("type") == "field")
      client_create_fields(msg);
  }
  if (msg.at("type") == "move")
    opponent_move(msg);

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
  if (!ws_p) {
    std::cerr << "Attempted write with no socket open\n";
    return;
  }

  ws_p->async_write(
      asio::buffer(json::serialize(val)),
      beast::bind_front_handler(&Multiplayer_ingame::on_write, this));
}

void Multiplayer_ingame::enter() {
  app->attach(output_box);
  if (game_type == MP_game_type::host)
    write_output("Awating connection at " + get_local_ip());
  else if (game_type == MP_game_type::client)
    write_output("Attempting to connect to " + address);
  app->attach(quit_button);
}

void Multiplayer_ingame::exit() {
  if (own_field)
    own_field->detach();
  if (opponent_field)
    opponent_field->detach();
  app->detach(output_box);
  app->detach(quit_button);
}

game_logic::IndexPair Multiplayer_ingame::server_create_fields() {
  own_field = std::make_unique<Field_widget>(
      app, settings, Graph_lib::Point{app->x_max() / 24, app->y_max() / 12},
      app->x_max() * 5 / 12, app->y_max() * 5 / 6,
      Cell_button::callback_multiplayer,
      std::bind(&Multiplayer_ingame::game_ended, this, std::placeholders::_1));
  own_field->field.generate_field({0, 0});

  game_logic::IndexPair start_pos{settings.count_rows / 2,
                                  settings.count_columns / 2};
  const game_logic::field_matrix_t &cells = own_field->field.get();
  for (int i = 0; i < settings.count_rows; ++i)
    for (int j = 0; j < settings.count_columns; ++j)
      if (cells[i][j].count_bomb == 0)
        start_pos = {i, j};

  own_field->attach();
  own_field->cell_interact(start_pos, Cell_interaction::open);

  opponent_field = std::make_unique<Field_widget>(
      app, own_field->field,
      Graph_lib::Point{app->x_max() * 13 / 24, app->y_max() / 12},
      app->x_max() * 5 / 12, app->y_max() * 5 / 6,
      [](Graph_lib::Address, Graph_lib::Address) {},
      std::bind(&Multiplayer_ingame::opponent_game_ended, this,
                std::placeholders::_1));
  opponent_field->attach();
  opponent_field->cell_interact(start_pos, Cell_interaction::open);

  return start_pos;
}

void Multiplayer_ingame::client_create_fields(json::value field_val) {
  json::value settings_val = field_val.at("settings").as_array();
  game_logic::Settings settings_received{
      settings_val.at(0).as_int64(),
      settings_val.at(1).as_int64(),
      settings_val.at(2).as_int64(),
  };
  settings = settings_received;

  std::vector<game_logic::IndexPair> bombs;
  for (auto elem : field_val.at("bombs").as_array())
    bombs.push_back({elem.at(0).as_int64(), elem.at(1).as_int64()});

  json::value start_pos_val = field_val.at("start_pos").as_array();
  game_logic::IndexPair start_pos{start_pos_val.at(0).as_int64(),
                                  start_pos_val.at(1).as_int64()};
  game_logic::Field base_field(settings);
  base_field.generate_field(bombs);

  own_field = std::make_unique<Field_widget>(
      app, base_field, Graph_lib::Point{app->x_max() / 24, app->y_max() / 12},
      app->x_max() * 5 / 12, app->y_max() * 5 / 6,
      Cell_button::callback_multiplayer,
      std::bind(&Multiplayer_ingame::game_ended, this, std::placeholders::_1));
  own_field->attach();
  own_field->cell_interact(start_pos, Cell_interaction::open);

  opponent_field = std::make_unique<Field_widget>(
      app, base_field,
      Graph_lib::Point{app->x_max() * 13 / 24, app->y_max() / 12},
      app->x_max() * 5 / 12, app->y_max() * 5 / 6,
      [](Graph_lib::Address, Graph_lib::Address) {},
      std::bind(&Multiplayer_ingame::opponent_game_ended, this,
                std::placeholders::_1));
  opponent_field->attach();
  opponent_field->cell_interact(start_pos, Cell_interaction::open);
}

void Multiplayer_ingame::opponent_move(json::value move_val) {
  json::value cell_indexes_val = move_val.at("cell_indexes").as_array();
  game_logic::IndexPair cell_indexes{cell_indexes_val.at(0).as_int64(),
                                     cell_indexes_val.at(1).as_int64()};
  if (move_val.at("interaction") == "open")
    opponent_field->cell_interact(cell_indexes, Cell_interaction::open);
  else if (move_val.at("interaction") == "mark")
    opponent_field->cell_interact(cell_indexes, Cell_interaction::mark);
}

void Multiplayer_ingame::close_connection() {
  if (ws_p)
    ws_p->async_close(websocket::close_code::none, [](beast::error_code) {});
}

void Multiplayer_ingame::clean_up(beast::error_code ec) {
  std::cerr << ec.message();
}

void Multiplayer_ingame::game_ended(Field_result res) {
  switch (res) {
  case Field_result::won:
    write_output("You win!");
    break;
  case Field_result::lost:
    write_output("You lose...");
    break;
  }
  close_connection();
};

void Multiplayer_ingame::opponent_game_ended(Field_result res) {
  game_ended(res == Field_result::won ? Field_result::lost : Field_result::won);
}

} // namespace minesweeper::app