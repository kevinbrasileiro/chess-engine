#pragma once

#include <array>

#include "../core/Board.hpp"

class Search {
public:
  static int searchPosition(Board& board, int depth, int alpha, int beta);
private:
};