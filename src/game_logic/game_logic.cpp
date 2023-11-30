#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

#include <game_logic/game_logic.h>

namespace minesweeper::game_logic {

Field::Field(Settings settings, IndexPair start) : settings{settings} {
  field_matrix_t field_temp(settings.count_columns,
                            std::vector<Cell>{settings.count_rows, Cell{0}});

  field = field_temp;
  generate_field(start);
}

void Field::open_cell(IndexPair cell) {
  field.at(cell.row).at(cell.column).open();
}

void Field::mark_cell(IndexPair cell) {
  field.at(cell.row).at(cell.column).mark();
}

void Field::generate_field(IndexPair start) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::vector<int> row_indexes;
  std::vector<int> column_indexes;

  for (int i = 0; i < settings.count_rows; i++)
    if (i != start.row)
      row_indexes.push_back(i);

  for (int i = 0; i < settings.count_columns; i++)
    if (i != start.column)
      column_indexes.push_back(i);

  std::shuffle(row_indexes.begin(), row_indexes.end(),
               std::default_random_engine(seed));
  std::shuffle(column_indexes.begin(), column_indexes.end(),
               std::default_random_engine(seed));

  for (int i = 0; i < settings.count_bomb; i++)
    field.at(row_indexes[i]).at(column_indexes[i]).count_bomb =
        9; // set cell as bomb
}

} // namespace minesweeper::game_logic