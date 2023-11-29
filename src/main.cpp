#include <game_logic/game_logic.h>
#include <iostream>

int main() {
  minesweeper::game_logic::Settings settings{2, 2, 1};
  minesweeper::game_logic::Field field{settings};

  field.mark_cell(0, 0);
  field.open_cell(0, 0);

  minesweeper::game_logic::field_matrix_t field_v = field.get_field();
  for (auto row : field_v) {
    for (auto i : row)
      std::cout << "is_marked: " << i.is_marked << " is_open: " << i.is_open
                << " ";

    std::cout << std::endl;
  }
}