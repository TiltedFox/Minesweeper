#ifndef APP_H
#define APP_H

#include <memory>
#include <string>

#include <Graph_lib/GUI.h>
#include <Graph_lib/Window.h>

#include <app/app_states.h>
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
  void test() { std::cout << "Successful callback! "; }

  // private:
  AppState *current_state;
};

class TestState1;
class TestState2;

class TestState2 : public AppState {
public:
  static AppState &get_instance() {
    static TestState2 singleton;
    return singleton;
  }

  void enter(App *app) override {
    app->attach(test_button);
    std::cout << "Enter state2\n";
  }
  void exit(App *app) override { app->detach(test_button); }

private:
  TestState2();
  Graph_lib::Button test_button;
};

class TestState1 : public AppState {
public:
  static AppState &get_instance() {
    static TestState1 singleton;
    return singleton;
  }

  void enter(App *app) override {
    app->attach(test_button);
    std::cout << "Enter state1\n";
  }
  void exit(App *app) override { app->detach(test_button); }

private:
  Graph_lib::Button test_button{
      Graph_lib::Point{20, 20}, 50, 50, "Change to state2",
      [](Graph_lib::Address, Graph_lib::Address button_addr) {
        auto *button = static_cast<Graph_lib::Button *>(button_addr);
        static_cast<App &>(button->window())
            .set_state(TestState2::get_instance());
      }};
};

} // namespace minesweeper::app
#endif // #ifndef GAME_H