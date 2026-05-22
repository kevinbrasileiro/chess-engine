#include <cmath> 
#include <array>
#include <algorithm>
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

  wKingPos = {4, 7};
  bKingPos = {4, 0};
}

bool Board::isInside(int file, int rank) const {
  return file >= 0 && file <= 7 && rank >= 0 && rank <= 7;
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
  if (move.movedPiece == W_KING) {
    wKingPos = move.to;
  } 
  if (move.movedPiece == B_KING) {
    bKingPos = move.to;
  }

  board[move.to.file][move.to.rank] = move.movedPiece;
  board[move.from.file][move.from.rank] = EMPTY;

  turn = getTurn() == WHITE ? BLACK : WHITE;
}

void Board::undoMove(const Move& move) {
  if (move.movedPiece == W_KING) {
    wKingPos = move.from;
  } 
  if (move.movedPiece == B_KING) {
    bKingPos = move.from;
  }

  board[move.from.file][move.from.rank] = move.movedPiece;
  board[move.to.file][move.to.rank] = move.capturedPiece;

  turn = getTurn() == WHITE ? BLACK : WHITE;
}

Position Board::findKing(Color color) {
  return color == WHITE ? wKingPos : bKingPos;
}

bool Board::isKingAttacked(Position kingPos) {
  Piece king = getPiece(kingPos);
  Color kingColor = getPieceColor(king);

  // PAWNS
  int direction = kingColor == WHITE ? -1 : 1;
  static const int captureOffests[2] = {-1, 1};

  for (const auto& offset : captureOffests) {
    int targetFile = kingPos.file + offset;
    int targetRank = kingPos.rank + direction;

    if (!isInside(targetFile, targetRank)) continue;

    Piece target = getPiece({targetFile, targetRank});
    Piece enemyPawn = kingColor == WHITE ? B_PAWN : W_PAWN;

    if (target == enemyPawn) return true;
  }
  
  // KNIGHT
  static const int jumps[8][2] = {
    {2, 1},
    {2, -1},
    {-2, 1},
    {-2, -1},
    {1, 2},
    {1, -2},
    {-1, 2},
    {-1, -2}
  };

  for (const auto& jump : jumps) {
    int targetFile = kingPos.file + jump[0];
    int targetRank = kingPos.rank + jump[1];

    if (!isInside(targetFile, targetRank)) continue;

    Piece target = getPiece({targetFile, targetRank});
    Piece enemyKnight = kingColor == WHITE ? B_KNIGHT : W_KNIGHT;

    if (target == enemyKnight) return true;
  }

  // SLIDING
  static const int directions[8][2] = {
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, 1},
  };

  for (const auto& direction : directions) {
    int currentFile = kingPos.file + direction[0];
    int currentRank = kingPos.rank + direction[1];

    while (isInside(currentFile, currentRank)) {
      Piece target = getPiece({currentFile, currentRank});
      
      if (target == EMPTY) {
        currentFile += direction[0];
        currentRank += direction[1];
        continue;
      }

      if (getPieceColor(target) == kingColor) break;

      bool diagonal = std::abs(direction[0]) == std::abs(direction[1]);
      bool adjacent = std::max(std::abs(currentFile - kingPos.file), std::abs(currentRank - kingPos.rank)) == 1;

      if (adjacent) {
        Piece enemyKing = kingColor == WHITE ? B_KING : W_KING;

        if (target == enemyKing) return true;
      }

      if (diagonal) {
        Piece enemyBishop = kingColor == WHITE ? B_BISHOP : W_BISHOP;
        Piece enemyQueen = kingColor == WHITE ? B_QUEEN : W_QUEEN;

        if (target == enemyBishop || target == enemyQueen) return true;

      } else {
        Piece enemyRook = kingColor == WHITE ? B_ROOK : W_ROOK;
        Piece enemyQueen = kingColor == WHITE ? B_QUEEN : W_QUEEN;

        if (target == enemyRook || target == enemyQueen) return true;
      }

      break;
    }
  }
  
  return false;
}
