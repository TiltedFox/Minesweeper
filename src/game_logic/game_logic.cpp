#include <cstdint>
#include <game_logic/game_logic.h>
#include <vector>

namespace minesweeper::game_logic {
Field::Field(Settings settings) : _settings{settings}
{
  field_matrix_t field_temp(
      settings.count_columns,
      std::vector<Cell>{settings.count_rows, Cell{0}});

  field = field_temp;
}

void Field::open_cell(uint8_t row, uint8_t column)
{
  field.at(row).at(column).open();
}

void Field::mark_cell(uint8_t row, uint8_t column)
{
  field.at(row).at(column).mark();
}

}  // namespace minesweeper::game_logic