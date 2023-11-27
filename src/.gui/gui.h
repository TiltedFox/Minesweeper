#ifndef GUI_H
#define GUI_H

namespace minesweeper::gui {

class Gui
{
public:
  void field ();
  void menu ();
  void win_screen ();
  void defeat_screen ();
  void button ();


private:
  //Painter* painter;
};

}  // namespace minesweeper::gui

#endif  // #ifndef GUI_H