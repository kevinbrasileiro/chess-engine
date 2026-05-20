#pragma once

#include "Types.hpp"
#include "Move.hpp"

class Board {
public:
  Board();

  Piece getPiece(Position pos) const;
  Color getPieceColor(Piece piece) const;
  Color getTurn() const;

  void makeMove(const Move& move);
  void undoMove(const Move& move);
  
private:
  Piece board[8][8];
  Color turn;

  void setupBoard();
};
