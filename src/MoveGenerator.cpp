#include "MoveGenerator.hpp"

std::vector<Move> MoveGenerator::generateMoves(const Board& board, Position pos) {
  return {
    {pos, {pos.file, pos.rank - 1}, board.getPiece(pos), EMPTY}
  };
}