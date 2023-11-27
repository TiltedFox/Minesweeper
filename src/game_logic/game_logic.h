#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <cstdint>
#include <vector>

namespace minesweeper::core::game_logic
{

  struct Settings
  {
    uint8_t _size;
    uint8_t _count_bomb;

    Settings(uint8_t size, uint8_t count_bomb)
        : _size{size}, _count_bomb{count_bomb} {};
  };

  enum class Difficulty
  {
    Undefined,
    Easy,
    Medium,
    Hard,
  };

  const Settings kEasy{8, 20};
  const Settings kMedium{10, 50};
  const Settings kHard{15, 100};

  struct Cell
  {
    uint8_t _count_bomb; // 9 means a bomb is in the cell
    bool is_marked{false};
    bool is_open{false};

    Cell(uint8_t count_bomb) : _count_bomb{count_bomb} {};

    void mark() { is_marked = true; };

    void unmark() { is_marked = false; };
  };

  class Field
  {
  public:
    Field(minesweeper::core::game_logic::Difficulty difficulty);

    void set_difficulty(Difficulty difficulty);
    void open_cell(uint8_t row, uint8_t column);
    void mark_cell(uint8_t row, uint8_t column);

    const std::vector<Cell> &get_field() { return field; };

  private:
    Settings _settings = kMedium;
    std::vector<Cell> field;
  };

} // namespace minesweeper::core::game_logic

#endif // #ifndef GAME_LOGIC_H