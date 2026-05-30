#include <cmath> 
#include <array>
#include <algorithm>
#include "Board.hpp"

constexpr const char* startingPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

Board::Board() {
  turn = WHITE;
  setupBoard(startingPosition);
}

void Board::setupBoard(const std::string& fen) {
  for (int square = 0; square < 64; square++) {
    board[square] = EMPTY;
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

    int sq = rank * 8 + file;
    board[sq] = piece;

    if (piece == W_KING)
      wKingPos = sq;

    if (piece == B_KING)
      bKingPos = sq;

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
    int file = enPassant[0] - 'a';
    int rank = enPassant[1] - '1';
    enPassantSquare = rank * 8 + file;
  } else {
    enPassantSquare = -1;
  }
}

void Board::makeMove(const Move& move) {
  if (move.movedPiece == W_KING) wKingPos = move.to;
  if (move.movedPiece == B_KING) bKingPos = move.to;

  Piece pieceToPlace = move.flag == PROMOTION ? move.promotedPiece : move.movedPiece;
  bool isWhite = getPieceColor(pieceToPlace) == WHITE;

  moveCastleRook(move);

  enPassantHistory.push_back(enPassantSquare);
  enPassantSquare = -1;

  int epDirection = isWhite ? 8 : -8; 

  if (pieceToPlace == W_PAWN || pieceToPlace == B_PAWN) {
    bool doubleMove = std::abs(move.from - move.to) == 16;
    if (doubleMove) enPassantSquare = move.to + epDirection;
  }

  if (move.flag == EN_PASSANT) {
    board[move.to + epDirection] = EMPTY;
  }
  
  castlingHistory.push_back(castlingRights);
  updateCastlingRights(move);

  board[move.to] = pieceToPlace;
  board[move.from] = EMPTY;

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

  enPassantSquare = enPassantHistory.back();
  enPassantHistory.pop_back();

  castlingRights = castlingHistory.back();
  castlingHistory.pop_back();

  if (move.flag == EN_PASSANT) {
    int direction = isWhite ? 8 : -8;

    board[move.to] = EMPTY;
    board[move.to + direction] = move.capturedPiece;
  } else {
    board[move.to] = move.capturedPiece;
  }
  
  board[move.from] = pieceToPlace;
  
  undoCastleRook(move);
  turn = (turn == WHITE) ? BLACK : WHITE;
}

void Board::updateCastlingRights(const Move& move) {
  switch (move.movedPiece) {
  case W_KING:
    castlingRights.whiteKingside = false;
    castlingRights.whiteQueenside = false;
    break;
  case B_KING:
    castlingRights.blackKingside = false;
    castlingRights.blackQueenside = false;
    break;

  case W_ROOK:
    if (move.from == 56) castlingRights.whiteQueenside = false;
    if (move.from == 63) castlingRights.whiteKingside = false;
    break;
  case B_ROOK:
    if (move.from == 0) castlingRights.blackQueenside = false;
    if (move.from == 7) castlingRights.blackKingside = false;
    break;
  
  default:
    break;
  }

  if (move.capturedPiece == W_ROOK) {
      if (move.to == 56) castlingRights.whiteQueenside = false;
      if (move.to == 63) castlingRights.whiteKingside = false;
  }

  if (move.capturedPiece == B_ROOK) {
      if (move.to == 0) castlingRights.blackQueenside = false;
      if (move.to == 7) castlingRights.blackKingside = false;
  }
}

void Board::moveCastleRook(const Move& move) {
  Piece rook = getPieceColor(move.movedPiece) == WHITE ? W_ROOK : B_ROOK;
  if (move.flag == CASTLE_KINGSIDE) {
    board[move.to + 1] = EMPTY;
    board[move.to - 1] = rook;
  } 
  if (move.flag == CASTLE_QUEENSIDE) {
    board[move.to - 2] = EMPTY;
    board[move.to + 1] = rook;
  }
}

void Board::undoCastleRook(const Move& move) {
  Piece rook = getPieceColor(move.movedPiece) == WHITE ? W_ROOK : B_ROOK;
  if (move.flag == CASTLE_KINGSIDE) {
    board[move.to + 1] = rook;
    board[move.to - 1] = EMPTY;
  } 
  if (move.flag == CASTLE_QUEENSIDE) {
    board[move.to - 2] = rook;
    board[move.to + 1] = EMPTY;
  }
}

int Board::findKing(Color color) const {
  return color == WHITE ? wKingPos : bKingPos;
}

bool Board::isSquareAttacked(int square, Color defenderColor) const {
  Piece enemyPawn = defenderColor == WHITE ? B_PAWN : W_PAWN;
  Piece enemyKnight = defenderColor == WHITE ? B_KNIGHT : W_KNIGHT;
  Piece enemyBishop = defenderColor == WHITE ? B_BISHOP : W_BISHOP;
  Piece enemyRook = defenderColor == WHITE ? B_ROOK : W_ROOK;
  Piece enemyQueen = defenderColor == WHITE ? B_QUEEN : W_QUEEN;
  Piece enemyKing = defenderColor == WHITE ? B_KING : W_KING;

  const auto& pawns = MoveTables::pawnAttacks[defenderColor == WHITE ? 0 : 1][square];
  for (int i = 0; i < pawns.count; i++) {
    if (getPiece(pawns.squares[i]) == enemyPawn) return true;
  }

  const auto& knights = MoveTables::knightMoves[square];
  for (int i = 0; i < knights.count; i++) {
    if (getPiece(knights.squares[i]) == enemyKnight) return true;
  }

  const auto& king = MoveTables::kingMoves[square];
  for (int i = 0; i < king.count; i++) {
    if (getPiece(king.squares[i]) == enemyKing) return true;
  }

  for (int dir = 0; dir < 4; dir++) {
    const auto& ray = MoveTables::bishopRays[square][dir];

    for (int i = 0; i < ray.count; i++) {
      int targetSquare = ray.squares[i];
      Piece targetPiece = getPiece(targetSquare);

      if (targetPiece != EMPTY) {
        if (targetPiece == enemyBishop || targetPiece == enemyQueen) return true;
        break;
      }
    }
  }

  for (int dir = 0; dir < 4; dir++) {
    const auto& ray = MoveTables::rookRays[square][dir];

    for (int i = 0; i < ray.count; i++) {
      int targetSquare = ray.squares[i];
      Piece targetPiece = getPiece(targetSquare);

      if (targetPiece != EMPTY) {
        if (targetPiece == enemyRook || targetPiece == enemyQueen) return true;
        break;
      }
    }
  }
  return false;
}
