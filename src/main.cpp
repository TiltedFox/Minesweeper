#include <app/app.h>

int main() {
  int w = 1000;
  int h = 600;

  minesweeper::app::App minesweeper{w, h, "Minesweeper"};
  Graph_lib::gui_main();
}
