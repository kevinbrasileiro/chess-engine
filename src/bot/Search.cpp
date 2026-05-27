#include "Search.hpp"
#include "Evaluation.hpp"

#include "../core/MoveGenerator.hpp"

int Search::searchPosition(Board& board, int depth, int alpha, int beta) {
  if (depth == 0) return Evaluation::evaluateBoard(board, board.getTurn());

  Color sideToMove = board.getTurn();

  MoveList moves;
  moves.clear();

  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {

      Piece piece = board.getPiece(file, rank);

      if (piece == EMPTY) continue;
      if (board.getPieceColor(piece) != sideToMove) continue;

      MoveGenerator::generateMoves(board, {file, rank}, moves);
    }
  }

  if (moves.count <= 0) {
    if (board.isSquareAttacked(board.findKing(sideToMove), sideToMove)) {
      return -1000000;
    }
    return 0;
  };

  int bestEval = -1000000;

  for (int i = 0; i < moves.count; i++) {
      const Move& move = moves[i];

      board.makeMove(move);
      int eval = -Search::searchPosition(board, depth - 1, -beta, -alpha);
      board.undoMove(move);

      if (eval > bestEval) {
        bestEval = eval;
      }
      
      if (eval >= beta) return beta;
      
      if (eval > alpha) {
        alpha = eval;
      }

  }
  
  return alpha;
}