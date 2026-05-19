#include <cmath> 
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

Color Board::getColor(Piece piece) {
  if (piece == EMPTY) return NO_COLOR;

  if (piece <= W_KING) return WHITE;

  return BLACK;
}

void Board::movePiece(Position from, Position to) {
  board[to.file][to.rank] = board[from.file][from.rank];
  
  turn = turn == WHITE ? BLACK : WHITE;

  board[from.file][from.rank] = EMPTY;
}

bool Board::isValidMove(Position from, Position to) {
  Piece piece = board[from.file][from.rank];
  Piece target = board[to.file][to.rank];

  if (getColor(piece) != turn) return false;

  if (getColor(piece) == getColor(target)) return false;
  if (from.file == to.file && from.rank == to.rank) return false;

  int dx = to.file - from.file;
  int dy = to.rank - from.rank;

  switch (piece) {
  
  case W_PAWN:
    if (from.rank == 6) {
      return dx == 0 && dy >= -2;
    } else {
      return dx == 0 && dy == -1;
    }
    break;

  case B_PAWN:
    if (from.rank == 1) {
      return dx == 0 && dy <= 2;
    } else {
      return dx == 0 && dy == 1;
    }
    break;

  case W_BISHOP:
  case B_BISHOP:
    return std::abs(dx) == std::abs(dy);
    break;

  case W_ROOK:
  case B_ROOK:
      return dx == 0 || dy == 0;
      break;

  case W_QUEEN:
  case B_QUEEN:
      return (dx == 0 || dy == 0) || (std::abs(dx) == std::abs(dy));
      break;

  case W_KNIGHT:
  case B_KNIGHT:
    return (std::abs(dx) == 2 && std::abs(dy) == 1) || (std::abs(dx) == 1 && std::abs(dy) == 2);
    break;
  
  case W_KING:
  case B_KING:
    return std::abs(dx) <= 1 && std::abs(dy) <= 1;
    break;

  default:
    return false;
  }
}