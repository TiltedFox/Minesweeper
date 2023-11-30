#include <cstdint>
#include <iostream>

#include <game_logic/game_logic.h>

int main() {
  minesweeper::game_logic::Settings settings{3, 4, 1};
  minesweeper::game_logic::IndexPair start{0, 0};
  minesweeper::game_logic::Field field{settings, start};

  const minesweeper::game_logic::field_matrix_t &field_v = field.get_field();
  for (auto row : field_v) {
    for (auto i : row)
      std::cout << static_cast<int>(i.count_bomb) << " | ";

    std::cout << std::endl;
  }
}