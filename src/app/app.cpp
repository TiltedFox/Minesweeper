#include <app/app.h>
#include <app/app_states.h>

<<<<<<< HEAD
namespace minesweeper::app {} // namespace minesweeper::app

void Win_window::easy() {}
void Win_window::medium() {}
void Win_window::hard() {}
void Win_window::custom() {}
void Win_window::start_game() {}

void Win_window::ip() {}


void Win_window::cb_quit(Graph_lib::Address , Graph_lib::Address pw) {
    auto* pb = static_cast<Graph_lib::Button*>(pw);
    static_cast<Win_window&>(pb->window()).quit();
}

void Win_window::cb_singleplayer(Graph_lib::Address , Graph_lib::Address pw) {
    auto* pb = static_cast<Graph_lib::Button*>(pw);
    static_cast<Win_window&>(pb->window()).singleplayer();
}

void Win_window::cb_multiplayer(Graph_lib::Address , Graph_lib::Address pw) {
    auto* pb = static_cast<Graph_lib::Button*>(pw);
    static_cast<Win_window&>(pb->window()).multiplayer();
}

void Win_window::cb_easy(Graph_lib::Address , Graph_lib::Address pw) {
    auto* pb = static_cast<Graph_lib::Button*>(pw);
    static_cast<Win_window&>(pb->window()).easy();
}

void Win_window::cb_medium(Graph_lib::Address , Graph_lib::Address pw) {
    auto* pb = static_cast<Graph_lib::Button*>(pw);
    static_cast<Win_window&>(pb->window()).medium();
}

void Win_window::cb_hard(Graph_lib::Address , Graph_lib::Address pw) {
    auto* pb = static_cast<Graph_lib::Button*>(pw);
    static_cast<Win_window&>(pb->window()).hard();
}

void Win_window::cb_custom(Graph_lib::Address , Graph_lib::Address pw) {
    auto* pb = static_cast<Graph_lib::Button*>(pw);
    static_cast<Win_window&>(pb->window()).custom();
}

void Win_window::cb_start_game(Graph_lib::Address , Graph_lib::Address pw) {
    auto* pb = static_cast<Graph_lib::Button*>(pw);
    static_cast<Win_window&>(pb->window()).start_game();
}

void Win_window::cb_ip(Graph_lib::Address , Graph_lib::Address pw) {
    auto* pb = static_cast<Graph_lib::Button*>(pw);
    static_cast<Win_window&>(pb->window()).ip();
}


void Win_window::singleplayer() {
    singleplayer_button.hide();
    multiplayer_button.hide();
    quit_button.hide();
    easy_dif.show();
    medium_dif.show();
    hard_dif.show();
    custom_dif.show();
    area_box.show();
    mines_box.show();
    start_game_button.show();
}

void Win_window::multiplayer() {
    singleplayer_button.hide();
    multiplayer_button.hide();
    quit_button.hide();
    ip_box.show();
    start_game_button.show();
}

void Win_window::quit() { hide(); }


Win_window::Win_window(Graph_lib::Point xy, int w, int h, int button_w, int button_h, const std::string& title)
    : Graph_lib::Window(xy, w, h, title),
    singleplayer_button(Graph_lib::Point{(w - button_w) / 2, h/2-20}, button_w, button_h, "Singleplayer", cb_singleplayer),
    multiplayer_button(Graph_lib::Point{(w - button_w) / 2, h/2+15}, button_w, button_h, "Multiplayer", cb_multiplayer),
    quit_button(Graph_lib::Point{(w - button_w) / 2, h/2+50}, button_w, button_h, "Quit", cb_quit),
    header(Graph_lib::Point{w * 4 / 10, h / 4}, "Minesweeper"),

    easy_dif(Graph_lib::Point{(w - button_w) / 3, h/2-20}, button_w, button_h, "Easy", cb_easy),
    medium_dif(Graph_lib::Point{(w - button_w) / 3, h/2+15}, button_w, button_h, "Meduim", cb_medium),
    hard_dif(Graph_lib::Point{(w - button_w) / 3, h/2+50}, button_w, button_h, "Hard", cb_hard),
    custom_dif(Graph_lib::Point{(w - button_w) * 2 / 3, h/2-20}, button_w, button_h, "Custom", cb_custom),
    area_box(Graph_lib::Point{(w - button_w) * 2 / 3, h/2+15}, button_w, button_h, "Area"),
    mines_box(Graph_lib::Point{(w - button_w) * 2 / 3, h/2+50}, button_w, button_h, "Mines"),
    start_game_button(Graph_lib::Point{(w - button_w) / 2, h * 5 / 6}, button_w, button_h, "Start game", cb_start_game),

    ip_box(Graph_lib::Point{(w - button_w) / 2, h * 2 / 3}, button_w, button_h, "IP-adress")

    {
        header.set_font_size(35);
        attach(singleplayer_button);
        attach(multiplayer_button);
        attach(quit_button);
        attach(header);
        attach(easy_dif);
        attach(medium_dif);
        attach(hard_dif);
        attach(custom_dif);
        attach(area_box);
        attach(mines_box);
        attach(start_game_button);
        attach(ip_box);
        easy_dif.hide();
        medium_dif.hide();
        hard_dif.hide();
        custom_dif.hide();
        area_box.hide();
        mines_box.hide();
        start_game_button.hide();
        ip_box.hide();
        
    }


int main() {

    int xmax = 600;
    int ymax = 400;

    int button_w = 100;
    int button_h = 30;

    Win_window win{Graph_lib::Point{100, 100}, xmax, ymax, button_w, button_h, "Minesweeper"};
    return Graph_lib::gui_main();

    return 0;
}
=======
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

} // namespace minesweeper::app
>>>>>>> dev
