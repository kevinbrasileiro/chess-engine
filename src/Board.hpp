#pragma once

#include "Types.hpp"
#include "Move.hpp"

#include <vector>

class Board {
public:
  Board();

  bool isInside(int file, int rank) const;

  Piece getPiece(Position pos) const;
  Color getPieceColor(Piece piece) const;
  Color getTurn() const;

  void makeMove(const Move& move);
  void undoMove(const Move& move);

  Position findKing(Color color) const;
  bool isKingAttacked(Position pos) const;

  int enPassantFile = -1;
  std::vector<int> enPassantHistory;
  
private:
  Piece board[8][8];
  Color turn;

  Position wKingPos;
  Position bKingPos;

  void setupBoard();
};
