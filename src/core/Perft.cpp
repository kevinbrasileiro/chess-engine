#include "Perft.hpp"
#include "MoveGenerator.hpp"

#include <iostream>

U64 perft(Board& board, int depth, bool root) {
  if (depth <= 0) return 1ULL;

  U64 totalNodes = 0;

  Color sideToMove = board.getTurn();

  MoveList moves;
  moves.clear();

  MoveGenerator::generateAllMoves(board, moves, sideToMove, false);

  for (int i = 0; i < moves.count; i++) {
    const Move& move = moves[i];
    board.makeMove(move);
    U64 nodes = perft(board, depth - 1, false);
    board.undoMove(move);

    totalNodes += nodes;

    if (root) std::cout << move.from << ": " << nodes << '\n';
  }

  if (root) std::cout << "\nTotal: " << totalNodes << '\n';
  return totalNodes;
}