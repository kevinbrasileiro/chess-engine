#include <cmath> 
#include "Board.hpp"

Board::Board() {
  turn = WHITE;
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

Piece Board::getPiece(Position pos) const {
  return board[pos.file][pos.rank];
}

Color Board::getPieceColor(Piece p) const {
  if (p == EMPTY) return NO_COLOR;
  return p <= W_KING ? WHITE : BLACK;
}

Color Board::getTurn() const {
  return turn;
}

void Board::makeMove(const Move& move) {
  board[move.to.file][move.to.rank] = move.movedPiece;
  board[move.from.file][move.from.rank] = EMPTY;

  turn = getTurn() == WHITE ? BLACK : WHITE;
}

void Board::undoMove(const Move& move) {
  board[move.from.file][move.from.rank] = move.movedPiece;
  board[move.to.file][move.to.rank] = move.capturedPiece;

  turn = getTurn() == WHITE ? BLACK : WHITE;
}

// bool Board::isValidMove(Position from, Position to) {
//   Piece piece = board[from.file][from.rank];
//   Piece target = board[to.file][to.rank];

//   if (getColor(piece) != turn) return false;

//   if (getColor(piece) == getColor(target)) return false;
//   if (from.file == to.file && from.rank == to.rank) return false;

//   int dx = to.file - from.file;
//   int dy = to.rank - from.rank;

//   switch (piece) {
  
//   case W_PAWN:
//     if (from.rank == 6) {
//       return dx == 0 && dy >= -2;
//     } else {
//       return dx == 0 && dy == -1;
//     }
//     break;

//   case B_PAWN:
//     if (from.rank == 1) {
//       return dx == 0 && dy <= 2;
//     } else {
//       return dx == 0 && dy == 1;
//     }
//     break;

//   case W_BISHOP:
//   case B_BISHOP:
//     return abs(dx) == abs(dy) && isPathClear(from, to);
//     break;

//   case W_ROOK:
//   case B_ROOK:
//       return (dx == 0 || dy == 0) && isPathClear(from, to);
//       break;

//   case W_QUEEN:
//   case B_QUEEN:
//       return ((dx == 0 || dy == 0) || (abs(dx) == abs(dy))) && isPathClear(from, to);
//       break;

//   case W_KNIGHT:
//   case B_KNIGHT:
//     return (abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2);
//     break;
  
//   case W_KING:
//   case B_KING:
//     return abs(dx) <= 1 && abs(dy) <= 1;
//     break;

//   default:
//     return false;
//   }
// }

// bool Board::isPathClear(Position from, Position to) {
//   int dx = to.file - from.file;
//   int dy = to.rank - from.rank;

//   int currentX = from.file;
//   int currentY = from.rank;

//   while (currentX != to.file || currentY != to.rank) {
//     if (dx != 0) currentX += dx > 0 ? 1 : -1;
//     if (dy != 0) currentY += dy > 0 ? 1 : -1;
    
//     if (board[currentX][currentY] != EMPTY) {
//       return false;
//     }
//   }
//   return true;
// }