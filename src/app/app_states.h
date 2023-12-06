#ifndef APP_STATES_H
#define APP_STATES_H

#include <app/app.h>

namespace minesweeper::app {
class TestState1;
class TestState2;

class TestState2 : public AppState {
public:
  TestState2(App *app);

  void enter() override {
    app->attach(test_button);
    std::cout << "Enter state2\n";
  }
  void exit() override { app->detach(test_button); }

private:
  Graph_lib::Button test_button;
};

class TestState1 : public AppState {
public:
  TestState1(App *app);

  void enter() override {
    app->attach(test_button);
    std::cout << "Enter state1\n";
  }
  void exit() override { app->detach(test_button); }

private:
  Graph_lib::Button test_button;
};
} // namespace minesweeper::app

#endif // #ifndef APP_STATES_H