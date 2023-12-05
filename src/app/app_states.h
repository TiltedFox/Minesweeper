#ifndef APP_STATES_H
#define APP_STATES_H

#include <app/app.h>

namespace minesweeper::app {
class App;
class AppState;

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
  TestState1();
  Graph_lib::Button test_button;
};
} // namespace minesweeper::app

#endif // #ifndef APP_STATES_H