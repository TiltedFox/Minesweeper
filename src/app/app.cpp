#include <app/app.h>

namespace minesweeper::app {
void App::set_state(AppState &new_state) {
  current_state->exit(this);
  current_state = &new_state;
  current_state->enter(this);
}

App::App(int w, int h, const std::string &title)
    : Graph_lib::Window(w, h, title) {
  current_state = &TestState1::get_instance();
  current_state->enter(this);
}

TestState2::TestState2()
    : test_button{Graph_lib::Point{100, 100}, 50, 50, "Change to state1",
                  [](Graph_lib::Address, Graph_lib::Address button_addr) {
                    auto *button =
                        static_cast<Graph_lib::Button *>(button_addr);
                    static_cast<App &>(button->window())
                        .set_state(TestState1::get_instance());
                  }} {};

} // namespace minesweeper::app
