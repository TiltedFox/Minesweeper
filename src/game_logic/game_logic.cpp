#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

#include <game_logic/game_logic.h>

using namespace std;

namespace minesweeper::game_logic {

Field::Field(Settings settings) : settings{settings} {}

Field::Field(const Field &field_copy)
    : settings{field_copy.get_settings()},
      field{static_cast<unsigned long>(settings.count_rows),
            vector<Cell>(static_cast<unsigned long>(settings.count_columns),
                         Cell{0})} {
  const field_matrix_t &data = field_copy.get();
  for (int i = 0; i < settings.count_rows; i++) {
    for (int j = 0; j < settings.count_columns; j++) {
      field[i][j] = data[i][j];
    }
  }
}

void Field::generate_field(IndexPair start) {
  generate_field(get_bomb_indexes(start));
}

void Field::generate_field(vector<IndexPair> bomb_indexes) {
  field = field_matrix_t(settings.count_rows,
                         vector<Cell>(settings.count_columns, Cell{0}));

  for (int i = 0; i < settings.count_bomb; i++) {
    int row = bomb_indexes[i].row;
    int column = bomb_indexes[i].column;

    field.at(row).at(column).count_bomb = 9; // set cell as bomb

    for (int j = row - 1; j <= row + 1; j++)
      for (int k = column - 1; k <= column + 1; k++) {
        if (!is_valid_index(j, k))
          continue;
        if (field.at(j).at(k).count_bomb == 9)
          continue;

        field.at(j).at(k).count_bomb += 1;
      }
  }
}

void Field::open_cell(IndexPair cell) {
  Cell &curr_cell = field.at(cell.row).at(cell.column);

  curr_cell.open();

  if (curr_cell.count_bomb == 0)
    for (int i = cell.row - 1; i <= cell.row + 1; i++)
      for (int j = cell.column - 1; j <= cell.column + 1; j++) {
        if (!is_valid_index(i, j))
          continue;
        if (field.at(i).at(j).is_open)
          continue;

        open_cell(IndexPair{i, j});
      }
}

void Field::mark_cell(IndexPair cell) {
  field.at(cell.row).at(cell.column).mark();
}

bool Field::is_bomb(IndexPair cell) {
  return field.at(cell.row).at(cell.column).count_bomb == 9;
}

bool Field::is_open(IndexPair cell) {
  return field.at(cell.row).at(cell.column).is_open;
}

bool Field::is_marked(IndexPair cell) {
  return field.at(cell.row).at(cell.column).is_marked;
}

bool Field::is_win() {
  int count_open = 0;
  int count_mark = 0;
  int amount_num =
      settings.count_rows * settings.count_columns - settings.count_bomb;
  for (int i = 0; i < settings.count_rows; i++) {
    for (int j = 0; j < settings.count_columns; j++) {
      if (is_open(IndexPair(i, j))) {
        count_open += 1;
      }
      if (is_marked(IndexPair(i, j)) && is_bomb(IndexPair(i, j))) {
        count_mark += 1;
      }
    }
  }
  return (count_open == amount_num && count_mark == settings.count_bomb);
}

vector<IndexPair> Field::get_bomb_indexes(IndexPair start) {
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  vector<IndexPair> bomb_indexes;

  for (int i = 0; i < settings.count_rows; i++)
    for (int j = 0; j < settings.count_columns; j++)
      if (i != start.row || j != start.column)
        bomb_indexes.push_back(IndexPair{i, j});

  shuffle(bomb_indexes.begin(), bomb_indexes.end(),
          default_random_engine(seed));

  return bomb_indexes;
}

bool Field::is_valid_index(int row, int column) {
  return (row >= 0 && column >= 0) &&
         (row < settings.count_rows && column < settings.count_columns);
}

} // namespace minesweeper::game_logic