#include "Evaluation.hpp"
#include <iostream>

inline int pstScore(Piece piece, int square) {
  switch (piece) {
    case W_PAWN:
    case B_PAWN:
      return PieceSquareTables::pawnTable[square];

    case W_KNIGHT:
    case B_KNIGHT: 
      return PieceSquareTables::knightTable[square];

    case W_BISHOP:
    case B_BISHOP:
      return PieceSquareTables::bishopTable[square];

    case W_ROOK: 
    case B_ROOK:
      return PieceSquareTables::rookTable[square];

    case W_QUEEN: 
    case B_QUEEN:
      return PieceSquareTables::queenTable[square];

    case W_KING:
    case B_KING:
      return PieceSquareTables::kingTable[square];

    default: 
      return 0;
  }
}

int Evaluation::evaluateBoard(Board& board, Color color) {
  int eval = 0;

  for (int square = 0; square < 64; square++) {
    Piece piece = board.getPiece(square);

    if (piece == EMPTY) continue;

    if (board.getPieceColor(piece) == WHITE) {
      eval += pieceValues[piece];
      eval += pstScore(piece, square);
    } else {
      eval -= pieceValues[piece - 6];
      eval -= pstScore(piece, square ^ 56);
    }
  }
  return color == WHITE ? eval : -eval;
}