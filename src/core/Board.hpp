#pragma once

#include "Types.hpp"
#include "Move.hpp"
#include "MoveTables.hpp"

#include <vector>

#include <sstream>
#include <string>
#include <cctype>

class Board {
public:
  Board();

  inline Piece getPiece(int square) const {
    return board[square];
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

  int findKing(Color color) const;
  bool isSquareAttacked(int square, Color defenderColor) const;

  int enPassantSquare = -1;
  std::vector<int> enPassantHistory;

  // CASTLING RIGHTS
  CastlingRights castlingRights;
  std::vector<CastlingRights> castlingHistory;

private:
  Piece board[64];
  Color turn;

  int wKingPos;
  int bKingPos;

  void updateCastlingRights(const Move& move);
  void moveCastleRook(const Move& move);
  void undoCastleRook(const Move& move);

  void setupBoard(const std::string& fen);
};
