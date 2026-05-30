#include "Evaluation.hpp"

int Evaluation::evaluateBoard(Board& board, Color color) {
  int eval = 0;

  for (int square = 0; square < 64; square++) {
    Piece piece = board.getPiece(square);

    if (piece == EMPTY) continue;

    if (board.getPieceColor(piece) == WHITE) {
      eval += pieceValues[piece];
    } else {
      eval -= pieceValues[piece - 6];
    }
  }
  return color == WHITE ? eval : -eval;
}