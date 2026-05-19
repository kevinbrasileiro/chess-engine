#pragma once

#include <vector>

#include "Board.hpp"

class MoveGenerator {
public:
  static std::vector<Move> generateMoves(const Board& board, Position pos);
private:
};