
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

class Board {
public:
  Board();
  Piece getPiece(int file, int rank) const;

  void movePiece(Position from, Position to);

  bool isValidMove(Position from, Position to);

private:
  Piece board[8][8];

  void setupBoard();
};
