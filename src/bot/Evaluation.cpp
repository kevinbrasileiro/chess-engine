#include "Evaluation.hpp"

int Evaluation::evaluateBoard(Board& board, Color color) {
  int eval = 0;

  for (int file = 0; file < 8; file++) {
    for (int rank = 0; rank < 8; rank++) {
      Piece piece = board.getPiece(file, rank);

      if (piece == EMPTY) continue;

      if (board.getPieceColor(piece) == WHITE) {
        eval += pieceValues[piece];
      } else {
        eval -= pieceValues[piece - 6];
      }
    }
  }
  return color == WHITE ? eval : -eval;
}