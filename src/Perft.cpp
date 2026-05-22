#include "Perft.hpp"
#include "MoveGenerator.hpp"

#include <iostream>

std::string squareToString(Position pos) {
  std::string s;
  s += char('a' + pos.file);
  s += char('8' - pos.rank);
  return s;
}

std::string moveToString(const Move& move) {
  return squareToString(move.from) + squareToString(move.to);
}

U64 perft(Board& board, int depth, bool root) {
  if (depth <= 0) return 1ULL;

  U64 totalNodes = 0;

  Color sideToMove = board.getTurn();

  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {

      Position pos{file, rank};
      Piece piece = board.getPiece(pos);

      if (piece == EMPTY) continue;

      if (board.getPieceColor(piece) != sideToMove) continue;

      std::vector<Move> moves = MoveGenerator::generateMoves(board, pos);

      for (const Move& move : moves) {
          board.makeMove(move);
          U64 nodes = perft(board, depth - 1, false);
          board.undoMove(move);

          totalNodes += nodes;

          if (root) std::cout << moveToString(move) << ": " << nodes << '\n';
      }
    }
  }
  if (root) std::cout << "\nTotal: " << totalNodes << '\n';
  return totalNodes;
}