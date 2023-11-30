#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <cstdint>
#include <vector>

namespace minesweeper::game_logic {

struct Settings {
  uint8_t count_rows;
  uint8_t count_columns;
  uint8_t count_bomb;

  Settings(uint8_t count_rows, uint8_t count_columns, uint8_t count_bomb)
      : count_rows{count_rows}, count_columns{count_columns},
        count_bomb{count_bomb} {};
};

const Settings kEasy{8, 8, 20};
const Settings kMedium{10, 10, 50};
const Settings kHard{15, 15, 100};

struct Cell {
  uint8_t count_bomb; // 9 means a bomb is in the cell
  bool is_marked{false};
  bool is_open{false};

  Cell(uint8_t count_bomb) : count_bomb{count_bomb} {};

  void open() { is_open = true; };

  void mark() { is_marked = is_marked ? false : true; };
};

using field_matrix_t = std::vector<std::vector<Cell>>;

struct IndexPair {
  uint8_t row;
  uint8_t column;

  IndexPair(uint8_t row, uint8_t column) : row{row}, column{column} {};
};

class Field {
public:
  Field(Settings settings, IndexPair start);

  void open_cell(IndexPair cell);
  void mark_cell(IndexPair cell);

  const field_matrix_t &get_field() { return field; };

private:
  Settings settings = kMedium;
  field_matrix_t field;

  void generate_field(IndexPair start);
};

} // namespace minesweeper::game_logic

#endif // #ifndef GAME_LOGIC_H