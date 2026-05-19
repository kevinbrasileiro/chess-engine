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

class Board {
public:
  Board();
  Piece getPiece(int rank, int file) const;

private:
  Piece board[8][8];

  void setupBoard();
};
