#pragma once

#include "Types.hpp"
#include "Move.hpp"

#include <vector>

#include <sstream>
#include <string>
#include <cctype>

class Board {
public:
  Board();

  inline bool isInside(int file, int rank) const {
    return file >= 0 && file <= 7 && rank >= 0 && rank <= 7;
  };

  inline Piece getPiece(int file, int rank) const {
    return board[file][rank];
  }
  inline Color getPieceColor(Piece piece) const {
    if (piece == EMPTY) return NO_COLOR;
    return piece <= W_KING ? WHITE : BLACK;
  }
  inline Color getTurn() const {
    return turn;
  };

  void makeMove(const Move& move);
  void undoMove(const Move& move);

  Position findKing(Color color) const;
  bool isSquareAttacked(Position pos, Color defenderColor) const;

  int enPassantFile = -1;
  std::vector<int> enPassantHistory;

  // CASTLING RIGHTS
  CastlingRights castlingRights;
  std::vector<CastlingRights> castlingHistory;

private:
  Piece board[8][8];
  Color turn;

  Position wKingPos;
  Position bKingPos;

  void updateCastlingRights(const Move& move);
  void moveCastleRook(const Move& move);
  void undoCastleRook(const Move& move);

  void setupBoard(const std::string& fen);
};
