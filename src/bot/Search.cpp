#include <algorithm>

#include "Search.hpp"
#include "Evaluation.hpp"

#include "../core/MoveGenerator.hpp"

int Search::searchPosition(Board& board, int depth, int alpha, int beta) {
  if (depth == 0) return Search::quiescence(board, alpha, beta);

  Color sideToMove = board.getTurn();

  MoveList moves;
  moves.clear();

  MoveGenerator::generateAllMoves(board, moves, sideToMove, false);

  if (moves.count <= 0) {
    if (board.isSquareAttacked(board.findKing(sideToMove), sideToMove)) {
      return -1000000;
    }
    return 0;
  };

  Search::orderMoves(moves);

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

int Search::quiescence(Board& board, int alpha, int beta) {
  int staticEval = Evaluation::evaluateBoard(board, board.getTurn());

  int bestValue = staticEval;
  
  if (bestValue >= beta) return bestValue;
  if (bestValue > alpha) {
    alpha = bestValue;
  }

  Color sideToMove = board.getTurn();

  MoveList moves;
  moves.clear();

  MoveGenerator::generateAllMoves(board, moves, sideToMove, true);
  Search::orderMoves(moves);

  for (int i = 0; i < moves.count; i++) {
      const Move& move = moves[i];

      board.makeMove(move);
      int score = -Search::quiescence(board, -beta, -alpha);
      board.undoMove(move);

      if (score >= beta) return score;
      if (score > bestValue) {
        bestValue = score;
      }
      if (score > alpha) {
        alpha = score;
      }
  }
  
  return bestValue;
};


int Search::scoreMove(const Move& move) {
  int score = 0;

  if (move.capturedPiece != EMPTY) {
    score = 10 * Evaluation::getPieceValue(move.capturedPiece) - Evaluation::getPieceValue(move.movedPiece);
  }

  if (move.flag == PROMOTION) {
    score += Evaluation::getPieceValue(move.promotedPiece);
  }

  return score;
}

void Search::orderMoves(MoveList& moves) {
  std::sort(moves.moves, moves.moves + moves.count, [](const Move& a, const Move& b) {
    return Search::scoreMove(a) > Search::scoreMove(b);
  });
}
