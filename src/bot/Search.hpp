#pragma once

#include <array>

#include "../core/Board.hpp"
#include "../core/MoveList.hpp"

class Search {
public:
  static int searchPosition(Board& board, int depth, int alpha, int beta);
  static void orderMoves(MoveList& moves);
private:
  static int scoreMove(const Move& move);
};