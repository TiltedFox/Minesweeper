#ifndef GAME_H
#define GAME_H

#include <game_logic/game_logic.h>

namespace minesweeper::game {

class Game
{
public:
  Game() = default;

  void start ();
  void stop ();
  void restart ();

private:
  minesweeper::game_logic::Field* field;

  void update_scene ();
  void make_move ();
};

}  // namespace minesweeper::game

#endif  // #ifndef GAME_H