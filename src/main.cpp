#include <Graph_lib/Simple_window.h>
#include <app/app.h>

int main() {
  minesweeper::app::App minesweeper{1000, 600, "Minesweeper"};
  Graph_lib::gui_main();
}
