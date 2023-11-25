#ifndef GAME_H
#define GAME_H

#include <core/game_logic/game_logic.h>

namespace minesweeper::game {


class Game
{
public:
  Game() = default;

  void start ();
  void stop ();
  void restart ();

private:
  minesweeper::core::game_logic::Field* field;
  minesweeper::gui::Gui* gui;

  void update_scene ();
  void make_move ();
};

}  // namespace minesweeper::game

#endif  // #ifndef GAME_H