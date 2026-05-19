#pragma once
struct Position {
  int file;
  int rank;
};

enum Piece {
  EMPTY, 

  W_PAWN,
  W_ROOK,
  W_KNIGHT,
  W_BISHOP,
  W_QUEEN,
  W_KING,

  B_PAWN,
  B_ROOK,
  B_KNIGHT,
  B_BISHOP,
  B_QUEEN,
  B_KING,
};

enum Color {
  NO_COLOR,
  WHITE,
  BLACK
};

class Board {
public:
  Board();
  Piece getPiece(int file, int rank) const;
  Color getColor(Piece piece);

  void movePiece(Position from, Position to);

  bool isValidMove(Position from, Position to);

private:
  Piece board[8][8];
  Color turn = WHITE;

  void setupBoard();
};
