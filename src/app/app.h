#ifndef APP_H
#define APP_H

#include <string>

#include <Graph_lib/GUI.h>
#include <Graph_lib/Window.h>

#include <game_logic/game_logic.h>

#include <iostream>

namespace minesweeper::app {
class App;

class AppState {
public:
  virtual void enter(App *app) = 0;
  virtual void exit(App *app) = 0;
  virtual ~AppState(){};
};

class App : public Graph_lib::Window {
public:
  App(int w, int h, const std::string &title);

  void set_state(AppState &new_state);

private:
  AppState *current_state;
};

inline App &get_app_ref(Graph_lib::Address widget_address) {
  auto *widget_p = static_cast<Graph_lib::Widget *>(widget_address);
  return static_cast<App &>(widget_p->window());
}

} // namespace minesweeper::app
#endif // #ifndef GAME_H