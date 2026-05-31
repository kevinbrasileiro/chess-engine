#include <iostream>
#include "MoveGenerator.hpp"

void MoveGenerator::generateAllMoves(Board& board, MoveList& allMoves, Color side, bool onlyCaptures) {  
  for (int square = 0; square < 64; square++) {
    Piece piece = board.getPiece(square);

    if (piece == EMPTY) continue;
    if (board.getPieceColor(piece) != side) continue;

    MoveGenerator::generatePieceMoves(board, square, allMoves, onlyCaptures);
  }
}

void MoveGenerator::generatePieceMoves(Board& board, int square, MoveList& legalMoves, bool onlyCaptures) {
  Piece piece = board.getPiece(square);

  switch (piece) {
    case W_PAWN:
    case B_PAWN:
      generatePawnMoves(board, square, legalMoves, onlyCaptures);
      break;

    case W_KNIGHT:
    case B_KNIGHT:
      generateKnightMoves(board, square, legalMoves, onlyCaptures);
      break;

    case W_BISHOP:
    case B_BISHOP:
      generateBishopMoves(board, square, legalMoves, onlyCaptures);
      break;

    case W_ROOK:
    case B_ROOK:
      generateRookMoves(board, square, legalMoves, onlyCaptures);
      break;

    case W_QUEEN:
    case B_QUEEN:
      generateBishopMoves(board, square, legalMoves, onlyCaptures);
      generateRookMoves(board, square, legalMoves, onlyCaptures);
      break;

    case W_KING:
    case B_KING:
      generateKingMoves(board, square, legalMoves, onlyCaptures);
      break;

    default:
      break;
  }
};

void MoveGenerator::generateKnightMoves(Board& board, int square, MoveList& moves, bool onlyCaptures) {
  auto& table = MoveTables::knightMoves[square];
  Piece piece = board.getPiece(square);

  for (int i = 0; i < table.count; i++) {
    int targetSquare = table.squares[i];

    Piece targetPiece = board.getPiece(targetSquare);

    if (targetPiece == EMPTY && !onlyCaptures) {
      tryMove(board, {square, targetSquare, piece, targetPiece}, moves);
      continue;
    }

    if (targetPiece != EMPTY && board.getPieceColor(targetPiece) != board.getPieceColor(piece)) {
      tryMove(board, {square, targetSquare, piece, targetPiece}, moves);
      continue;
    }
  }
}

void MoveGenerator::generateBishopMoves(Board& board, int square, MoveList& moves, bool onlyCaptures) {
  Piece piece = board.getPiece(square);

  for (int dir = 0; dir < 4; dir++) {
    const auto& ray = MoveTables::bishopRays[square][dir];

    for (int i = 0; i < ray.count; i++) {
      int targetSquare = ray.squares[i];
      Piece targetPiece = board.getPiece(targetSquare);
  
      if (targetPiece == EMPTY && !onlyCaptures) {
        tryMove(board, {square, targetSquare, piece, targetPiece}, moves);
        continue;
      }

      if (targetPiece != EMPTY) {
        if (board.getPieceColor(targetPiece) != board.getPieceColor(piece)) {
          tryMove(board, {square, targetSquare, piece, targetPiece}, moves);
        }
        break;
      }
    }
  }
}

void MoveGenerator::generateRookMoves(Board& board, int square, MoveList& moves, bool onlyCaptures) {
  Piece piece = board.getPiece(square);

  for (int dir = 0; dir < 4; dir++) {
    const auto& ray = MoveTables::rookRays[square][dir];

    for (int i = 0; i < ray.count; i++) {
      int targetSquare = ray.squares[i];
      Piece targetPiece = board.getPiece(targetSquare);
  
      if (targetPiece == EMPTY && !onlyCaptures) {
        tryMove(board, {square, targetSquare, piece, targetPiece}, moves);
        continue;
      }

      if (targetPiece != EMPTY) {
        if (board.getPieceColor(targetPiece) != board.getPieceColor(piece)) {
          tryMove(board, {square, targetSquare, piece, targetPiece}, moves);
        }
        break;
      }
    }
  }
}

void MoveGenerator::generateKingMoves(Board& board, int square, MoveList& moves, bool onlyCaptures) {
  auto& table = MoveTables::kingMoves[square];
  Piece piece = board.getPiece(square);

  for (int i = 0; i < table.count; i++) {
    int targetSquare = table.squares[i];

    Piece targetPiece = board.getPiece(targetSquare);

    if (targetPiece == EMPTY && !onlyCaptures) {
      tryMove(board, {square, targetSquare, piece, targetPiece}, moves);
      continue;
    }

    if (targetPiece != EMPTY && board.getPieceColor(targetPiece) != board.getPieceColor(piece)) {
      tryMove(board, {square, targetSquare, piece, targetPiece}, moves);
      continue;
    }
  }

  if (onlyCaptures) return;

  if (piece == W_KING && square == 60) {
    if (board.castlingRights.whiteKingside) {
      bool isPathEmpty = board.getPiece(61) == EMPTY && board.getPiece(62) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked(60, WHITE) && !board.isSquareAttacked(61, WHITE) && !board.isSquareAttacked(62, WHITE);
      bool isRookThere = board.getPiece(63) == W_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) tryMove(board, {square, 62, piece, EMPTY, CASTLE_KINGSIDE}, moves); 
    }

    if (board.castlingRights.whiteQueenside) {
      bool isPathEmpty = board.getPiece(57) == EMPTY && board.getPiece(58) == EMPTY && board.getPiece(59) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked(58, WHITE) && !board.isSquareAttacked(59, WHITE) && !board.isSquareAttacked(60, WHITE);
      bool isRookThere = board.getPiece(56) == W_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) tryMove(board, {square, 58, piece, EMPTY, CASTLE_QUEENSIDE}, moves); 
    }
  }

  if (piece == B_KING && square == 4) {
    if (board.castlingRights.blackKingside) {
      bool isPathEmpty = board.getPiece(5) == EMPTY && board.getPiece(6) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked(4, BLACK) && !board.isSquareAttacked(5, BLACK) && !board.isSquareAttacked(6, BLACK);
      bool isRookThere = board.getPiece(7) == B_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) tryMove(board, {square, 6, piece, EMPTY, CASTLE_KINGSIDE}, moves); 
    }

    if (board.castlingRights.blackQueenside) {
      bool isPathEmpty = board.getPiece(1) == EMPTY && board.getPiece(2) == EMPTY && board.getPiece(3) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked(2, BLACK) && !board.isSquareAttacked(3, BLACK) && !board.isSquareAttacked(4, BLACK);
      bool isRookThere = board.getPiece(0) == B_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) tryMove(board, {square, 2, piece, EMPTY, CASTLE_QUEENSIDE}, moves); 
    }
  }
}

void MoveGenerator::generatePawnMoves(Board& board, int square, MoveList& moves, bool onlyCaptures) {
  Piece piece = board.getPiece(square);
  bool isWhite = board.getPieceColor(piece) == WHITE;

  int direction = isWhite ? -8 : 8;

  int startRank = isWhite ? 6 : 1;
  int promoRank = isWhite ? 1 : 6;

  int rank = square / 8;
  int file = square % 8;

  int oneStep = square + direction;
  int twoStep = square + direction * 2;

  if (oneStep < 0 || oneStep >= 64) return;

  if (board.getPiece(oneStep) == EMPTY && !onlyCaptures) {
    if (rank == promoRank) {
      tryMove(board, {square, oneStep, piece, EMPTY, PROMOTION, isWhite ? W_QUEEN : B_QUEEN}, moves);
      tryMove(board, {square, oneStep, piece, EMPTY, PROMOTION, isWhite ? W_ROOK : B_ROOK}, moves);
      tryMove(board, {square, oneStep, piece, EMPTY, PROMOTION, isWhite ? W_BISHOP : B_BISHOP}, moves);
      tryMove(board, {square, oneStep, piece, EMPTY, PROMOTION, isWhite ? W_KNIGHT : B_KNIGHT}, moves);
    } else {
      tryMove(board, {square, oneStep, piece, EMPTY}, moves);
    }

    if (rank == startRank && board.getPiece(twoStep) == EMPTY) {
      tryMove(board, {square, twoStep, piece, EMPTY}, moves);
    }
  }

  const auto& table = MoveTables::pawnAttacks[isWhite ? 0 : 1][square];
  for (int i = 0; i < table.count; i++) {
    int targetSquare = table.squares[i];

    Piece targetPiece = board.getPiece(targetSquare);

    if (targetPiece != EMPTY && board.getPieceColor(targetPiece) != board.getPieceColor(piece)) {
      if (rank == promoRank) {
        tryMove(board, {square, targetSquare, piece, targetPiece, PROMOTION, isWhite ? W_QUEEN : B_QUEEN}, moves);
        tryMove(board, {square, targetSquare, piece, targetPiece, PROMOTION, isWhite ? W_ROOK : B_ROOK}, moves);
        tryMove(board, {square, targetSquare, piece, targetPiece, PROMOTION, isWhite ? W_BISHOP : B_BISHOP}, moves);
        tryMove(board, {square, targetSquare, piece, targetPiece, PROMOTION, isWhite ? W_KNIGHT : B_KNIGHT}, moves);
      } else {
        tryMove(board, {square, targetSquare, piece, targetPiece}, moves);
      }
    }

    if (targetPiece == EMPTY && targetSquare == board.enPassantSquare) {
      tryMove(board, {square, targetSquare, piece, isWhite ? B_PAWN : W_PAWN, EN_PASSANT}, moves);
    }
  }
}
