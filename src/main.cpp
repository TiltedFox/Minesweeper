#include <Graph_lib/Simple_window.h>
#include <app/app.h>

int main() {
  minesweeper::app::App minesweeper{640, 480, "Minesweeper"};
  Graph_lib::gui_main();
}
