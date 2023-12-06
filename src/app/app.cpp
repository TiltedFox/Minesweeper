#include <app/app.h>
#include <app/app_states.h>

namespace minesweeper::app {
void App::set_state(AppState &new_state) {
  current_state->exit(this);
  current_state = &new_state;
  current_state->enter(this);
}

App::App(int w, int h, const std::string &title)
    : Graph_lib::Window(w, h, title) {
  current_state = &Main_menu::get_instance();
  current_state->enter(this);
}

} // namespace minesweeper::app
