#include "Board.hpp"

Board::Board() {
  setupBoard();
}

void Board::setupBoard() {
  for (int file = 0; file < 8; file++) {
    for (int rank = 0; rank < 8; rank++) {
      board[file][rank] = EMPTY;
    }
  }

  for (int file = 0; file < 8; file++) {
    board[file][1] = B_PAWN;
    board[file][6] = W_PAWN;
  }

  board[0][0] = B_ROOK;
  board[1][0] = B_KNIGHT;
  board[2][0] = B_BISHOP;
  board[3][0] = B_QUEEN;
  board[4][0] = B_KING;
  board[5][0] = B_BISHOP;
  board[6][0] = B_KNIGHT;
  board[7][0] = B_ROOK;

  board[0][7] = W_ROOK;
  board[1][7] = W_KNIGHT;
  board[2][7] = W_BISHOP;
  board[3][7] = W_QUEEN;
  board[4][7] = W_KING;
  board[5][7] = W_BISHOP;
  board[6][7] = W_KNIGHT;
  board[7][7] = W_ROOK;
}

Piece Board::getPiece(int file, int rank) const {
  return board[file][rank];
}

void Board::movePiece(Position from, Position to) {
  board[to.file][to.rank] = board[from.file][from.rank];

  board[from.file][from.rank] = EMPTY;
}