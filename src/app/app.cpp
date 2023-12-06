#include <app/app.h>
#include <app/app_states.h>

namespace minesweeper::app {
void App::set_state(AppState *new_state) {
  current_state->exit();
  current_state.reset(new_state);
  current_state->enter();
}

App::App(int w, int h, const std::string &title)
    : Graph_lib::Window(w, h, title),
      current_state{std::make_unique<TestState1>(this)} {
  current_state->enter();
}

} // namespace minesweeper::app
