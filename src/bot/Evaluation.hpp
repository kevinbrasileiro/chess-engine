#pragma once

#include <array>

#include "../core/Board.hpp"

class Evaluation {
public:
  static int evaluateBoard(Board& board, Color color);
private:
  static constexpr std::array<int, 7> pieceValues{
    0, // empty
    100, // pawn
    300, // knight
    320, // bishop
    500, // rook
    900, // queen
    10000, // king
  };
};