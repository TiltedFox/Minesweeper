#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <vector>

namespace minesweeper::game_logic {

struct Settings {
  int count_rows;
  int count_columns;
  int count_bomb;

  Settings(int count_rows, int count_columns, int count_bomb)
      : count_rows{count_rows}, count_columns{count_columns},
        count_bomb{count_bomb} {};
};

const Settings kEasy{8, 8, 10};
const Settings kMedium{10, 10, 25};
const Settings kHard{15, 15, 50};

struct Cell {
  int count_bomb; // 9 means a bomb is in the cell
  bool is_marked{false};
  bool is_open{false};

  Cell(int count_bomb) : count_bomb{count_bomb} {};

  void open() { is_open = true; };

  void mark() { is_marked = is_marked ? false : true; };
};

struct IndexPair {
  int row;
  int column;

  IndexPair(int row, int column) : row{row}, column{column} {};
};

using field_matrix_t = std::vector<std::vector<Cell>>;

class Field {
public:
  Field(Settings settings);

  void generate_field(IndexPair start);
  void open_cell(IndexPair cell);
  void mark_cell(IndexPair cell);
  bool is_bomb(IndexPair cell);
  bool is_open(IndexPair cell);
  bool is_marked(IndexPair cell);

  const field_matrix_t &get() { return field; };

private:
  Settings settings = kMedium;
  field_matrix_t field;

  bool is_valid_index(int row, int column);
  std::vector<IndexPair> get_bomb_indexes(IndexPair start);
};

} // namespace minesweeper::game_logic

#endif // #ifndef GAME_LOGIC_H