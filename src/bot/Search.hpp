#pragma once

#include <array>

#include "../core/Board.hpp"
#include "../core/MoveList.hpp"

class Search {
public:
  static int searchPosition(Board& board, int depth, int alpha, int beta);
  static int quiescence(Board& board, int alpha, int beta);
  static void orderMoves(MoveList& moves, const Board& board);
private:
  static int scoreMove(const Move& move, const Board& board);
};