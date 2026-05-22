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

  castlingRights = {true, true, true, true};
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
  if (move.movedPiece == W_KING) wKingPos = move.to;
  if (move.movedPiece == B_KING) bKingPos = move.to;

  Piece pieceToPlace = move.movedPiece;
  bool isWhite = getPieceColor(pieceToPlace) == WHITE;

  if (move.flag == PROMOTION) {
    pieceToPlace = isWhite ? W_QUEEN : B_QUEEN;
  }

  enPassantHistory.push_back(enPassantFile);
  enPassantFile = -1;
  if (pieceToPlace == W_PAWN || pieceToPlace == B_PAWN) {
    bool doubleMove = std::abs(move.from.rank - move.to.rank) == 2;
    if (doubleMove) enPassantFile = move.from.file;
  }

  if (move.flag == EN_PASSANT) {
    int direction = isWhite ? 1 : -1;

    board[move.to.file][move.to.rank + direction] = EMPTY;
  }
  
  castlingHistory.push_back(castlingRights);
  updateCastlingRights(move);
  moveCastleRook(move);

  board[move.to.file][move.to.rank] = pieceToPlace;
  board[move.from.file][move.from.rank] = EMPTY;

  turn = (turn == WHITE) ? BLACK : WHITE;
}

void Board::undoMove(const Move& move) {
  if (move.movedPiece == W_KING) wKingPos = move.from;
  if (move.movedPiece == B_KING) bKingPos = move.from;

  Piece pieceToPlace = move.movedPiece;
  bool isWhite = getPieceColor(pieceToPlace) == WHITE;

  if (move.flag == PROMOTION) {
    pieceToPlace = isWhite ? W_PAWN : B_PAWN;
  }

  enPassantFile = enPassantHistory.back();
  enPassantHistory.pop_back();
  if (move.flag == EN_PASSANT) {
    int direction = isWhite ? 1 : -1;

    board[move.to.file][move.to.rank] = EMPTY;
    board[move.to.file][move.to.rank + direction] = move.capturedPiece;
  } else {
    board[move.to.file][move.to.rank] = move.capturedPiece;
  }

  castlingRights = castlingHistory.back();
  castlingHistory.pop_back();
  undoCastleRook(move);
  
  board[move.from.file][move.from.rank] = pieceToPlace;

  turn = (turn == WHITE) ? BLACK : WHITE;
}

void Board::updateCastlingRights(const Move& move) {
  Piece pieceMoved = move.movedPiece;

  switch (pieceMoved) {
  case W_KING:
    castlingRights.whiteKingside = false;
    castlingRights.whiteQueenside = false;
    break;
  case B_KING:
    castlingRights.blackKingside = false;
    castlingRights.blackQueenside = false;
    break;

  case W_ROOK:
    if (move.from.file == 0 && move.from.rank == 7) castlingRights.whiteQueenside = false;
    if (move.from.file == 7 && move.from.rank == 7) castlingRights.whiteKingside = false;
    break;
  case B_ROOK:
    if (move.from.file == 0 && move.from.rank == 0) castlingRights.blackQueenside = false;
    if (move.from.file == 7 && move.from.rank == 0) castlingRights.blackKingside = false;
    break;
  
  default:
    break;
  }
}

void Board::moveCastleRook(const Move& move) {
  Piece rook = getPieceColor(move.movedPiece) == WHITE ? W_ROOK : B_ROOK;
  if (move.flag == CASTLE_KINGSIDE) {
    board[7][move.to.rank] = EMPTY;
    board[5][move.to.rank] = rook;
  } 
  if (move.flag == CASTLE_QUEENSIDE) {
    board[0][move.to.rank] = EMPTY;
    board[3][move.to.rank] = rook;
  }
}

void Board::undoCastleRook(const Move& move) {
  Piece rook = getPieceColor(move.movedPiece) == WHITE ? W_ROOK : B_ROOK;
  if (move.flag == CASTLE_KINGSIDE) {
    board[7][move.to.rank] = rook;
    board[5][move.to.rank] = EMPTY;
  } 
  if (move.flag == CASTLE_QUEENSIDE) {
    board[0][move.to.rank] = rook;
    board[3][move.to.rank] = EMPTY;
  }
}

Position Board::findKing(Color color) const {
  return color == WHITE ? wKingPos : bKingPos;
}

bool Board::isSquareAttacked(Position pos, Color defenderColor) const {
  // PAWNS
  int direction = defenderColor == WHITE ? -1 : 1;
  static const int captureOffests[2] = {-1, 1};

  for (const auto& offset : captureOffests) {
    int targetFile = pos.file + offset;
    int targetRank = pos.rank + direction;

    if (!isInside(targetFile, targetRank)) continue;

    Piece target = getPiece({targetFile, targetRank});
    Piece enemyPawn = defenderColor == WHITE ? B_PAWN : W_PAWN;

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
    int targetFile = pos.file + jump[0];
    int targetRank = pos.rank + jump[1];

    if (!isInside(targetFile, targetRank)) continue;

    Piece target = getPiece({targetFile, targetRank});
    Piece enemyKnight = defenderColor == WHITE ? B_KNIGHT : W_KNIGHT;

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
    int currentFile = pos.file + direction[0];
    int currentRank = pos.rank + direction[1];

    while (isInside(currentFile, currentRank)) {
      Piece target = getPiece({currentFile, currentRank});
      
      if (target == EMPTY) {
        currentFile += direction[0];
        currentRank += direction[1];
        continue;
      }

      if (getPieceColor(target) == defenderColor) break;

      bool diagonal = std::abs(direction[0]) == std::abs(direction[1]);
      bool adjacent = std::max(std::abs(currentFile - pos.file), std::abs(currentRank - pos.rank)) == 1;

      if (adjacent) {
        Piece enemyKing = defenderColor == WHITE ? B_KING : W_KING;

        if (target == enemyKing) return true;
      }

      if (diagonal) {
        Piece enemyBishop = defenderColor == WHITE ? B_BISHOP : W_BISHOP;
        Piece enemyQueen = defenderColor == WHITE ? B_QUEEN : W_QUEEN;

        if (target == enemyBishop || target == enemyQueen) return true;

      } else {
        Piece enemyRook = defenderColor == WHITE ? B_ROOK : W_ROOK;
        Piece enemyQueen = defenderColor == WHITE ? B_QUEEN : W_QUEEN;

        if (target == enemyRook || target == enemyQueen) return true;
      }

      break;
    }
  }
  
  return false;
}
