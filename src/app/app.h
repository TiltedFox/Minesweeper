#ifndef APP_H
#define APP_H

#include <memory>
#include <string>
#include <thread>

// don't move below graph lib unless you want an error
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/bind/bind.hpp>
#include <boost/json.hpp>
// #include <boost/thread/thread.hpp>
// #include <boost/chrono.hpp>

#include <Graph_lib/GUI.h>
#include <Graph_lib/Window.h>

#include <game_logic/game_logic.h>

#include <iostream>

namespace minesweeper::app {
class App;

class AppState {
public:
  AppState(App *app) : app{app} {};
  virtual void enter() = 0;
  virtual void exit() = 0;
  virtual ~AppState(){};

protected:
  App *app;
};

class App : public Graph_lib::Window {
public:
  App(int w, int h, const std::string &title);

  void set_state(AppState *new_state);

  AppState &get_state() { return *current_state; };

private:
  std::unique_ptr<AppState> current_state;
};

inline App &get_app_ref(Graph_lib::Address widget_address) {
  auto *widget_p = static_cast<Graph_lib::Widget *>(widget_address);
  return static_cast<App &>(widget_p->window());
}

inline Graph_lib::Button &get_button_ref(Graph_lib::Address button_address) {
  return *static_cast<Graph_lib::Button *>(button_address);
}

} // namespace minesweeper::app
#endif // #ifndef GAME_H