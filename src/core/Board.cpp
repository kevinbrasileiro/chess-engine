#include <cmath> 
#include <array>
#include <algorithm>
#include "Board.hpp"

Board::Board() {
  turn = WHITE;
  setupBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void Board::setupBoard(const std::string& fen) {
  for (int file = 0; file < 8; file++) {
    for (int rank = 0; rank < 8; rank++) {
      board[file][rank] = EMPTY;
    }
  }

  std::stringstream ss(fen);

  std::string placement;
  std::string activeColor;
  std::string castling;
  std::string enPassant;

  ss >> placement >> activeColor >> castling >> enPassant;

  int rank = 0;
  int file = 0;

  for (char c : placement) {
    if (c == '/') {
      rank++;
      file = 0;
      continue;
    }

    if (std::isdigit(c)) {
      file += c - '0';
      continue;
    }

    Piece piece = EMPTY;

    switch (c) {
      case 'P': piece = W_PAWN; break;
      case 'N': piece = W_KNIGHT; break;
      case 'B': piece = W_BISHOP; break;
      case 'R': piece = W_ROOK; break;
      case 'Q': piece = W_QUEEN; break;
      case 'K': piece = W_KING; break;

      case 'p': piece = B_PAWN; break;
      case 'n': piece = B_KNIGHT; break;
      case 'b': piece = B_BISHOP; break;
      case 'r': piece = B_ROOK; break;
      case 'q': piece = B_QUEEN; break;
      case 'k': piece = B_KING; break;
    }

    board[file][rank] = piece;

    if (piece == W_KING)
      wKingPos = {file, rank};

    if (piece == B_KING)
      bKingPos = {file, rank};

    file++;
  }

  turn = activeColor == "w" ? WHITE : BLACK;

  castlingRights = {false, false, false, false};

  for (char c : castling) {
    switch (c) {
      case 'K': castlingRights.whiteKingside = true; break;
      case 'Q': castlingRights.whiteQueenside = true; break;
      case 'k': castlingRights.blackKingside = true; break;
      case 'q': castlingRights.blackQueenside = true; break;
    }
  }

  if (enPassant != "-") {
    enPassantFile = enPassant[0] - 'a';
  } else {
    enPassantFile = -1;
  }
}

void Board::makeMove(const Move& move) {
  if (move.movedPiece == W_KING) wKingPos = move.to;
  if (move.movedPiece == B_KING) bKingPos = move.to;

  Piece pieceToPlace = move.movedPiece;
  bool isWhite = getPieceColor(pieceToPlace) == WHITE;

  switch (move.flag) {
  case PROMOTION_QUEEN:
    pieceToPlace = isWhite ? W_QUEEN : B_QUEEN;
    break;
  case PROMOTION_ROOK:
    pieceToPlace = isWhite ? W_ROOK : B_ROOK;
    break;
  case PROMOTION_BISHOP:
    pieceToPlace = isWhite ? W_BISHOP : B_BISHOP;
    break;
  case PROMOTION_KNIGHT:
    pieceToPlace = isWhite ? W_KNIGHT : B_KNIGHT;
    break;
  default:
    break;
  }

  moveCastleRook(move);

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

  board[move.to.file][move.to.rank] = pieceToPlace;
  board[move.from.file][move.from.rank] = EMPTY;

  turn = (turn == WHITE) ? BLACK : WHITE;
}

void Board::undoMove(const Move& move) {
  if (move.movedPiece == W_KING) wKingPos = move.from;
  if (move.movedPiece == B_KING) bKingPos = move.from;

  Piece pieceToPlace = move.movedPiece;
  bool isWhite = getPieceColor(pieceToPlace) == WHITE;

  if (move.flag >= PROMOTION_QUEEN) {
    pieceToPlace = isWhite ? W_PAWN : B_PAWN;
  }

  enPassantFile = enPassantHistory.back();
  enPassantHistory.pop_back();

  castlingRights = castlingHistory.back();
  castlingHistory.pop_back();

  if (move.flag == EN_PASSANT) {
    int direction = isWhite ? 1 : -1;

    board[move.to.file][move.to.rank] = EMPTY;
    board[move.to.file][move.to.rank + direction] = move.capturedPiece;
  } else {
    board[move.to.file][move.to.rank] = move.capturedPiece;
  }
  
  board[move.from.file][move.from.rank] = pieceToPlace;
  
  undoCastleRook(move);
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

  if (move.capturedPiece == W_ROOK) {
      if (move.to.file == 0 && move.to.rank == 7) castlingRights.whiteQueenside = false;
      if (move.to.file == 7 && move.to.rank == 7) castlingRights.whiteKingside = false;
  }

  if (move.capturedPiece == B_ROOK) {
      if (move.to.file == 0 && move.to.rank == 0) castlingRights.blackQueenside = false;
      if (move.to.file == 7 && move.to.rank == 0) castlingRights.blackKingside = false;
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

    Piece target = getPiece(targetFile, targetRank);
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

    Piece target = getPiece(targetFile, targetRank);
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
      Piece target = getPiece(currentFile, currentRank);
      
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
