#include <iostream>
#include "MoveGenerator.hpp"

void MoveGenerator::generateAllMoves(Board& board, MoveList& allMoves, Color side, bool onlyCaptures) {  
  for (int rank = 0; rank < 8; ++rank) {
    for (int file = 0; file < 8; ++file) {

      Piece piece = board.getPiece(file, rank);

      if (piece == EMPTY) continue;
      if (board.getPieceColor(piece) != side) continue;

      MoveGenerator::generatePieceMoves(board, {file, rank}, allMoves, onlyCaptures);
    }
  }
}

void MoveGenerator::generatePieceMoves(Board& board, Position pos, MoveList& legalMoves, bool onlyCaptures) {
  Piece piece = board.getPiece(pos.file, pos.rank);

  switch (piece) {
    case W_PAWN:
    case B_PAWN:
      generatePawnMoves(board, pos, legalMoves, onlyCaptures);
      break;

    case W_KNIGHT:
    case B_KNIGHT:
      generateKnightMoves(board, pos, legalMoves, onlyCaptures);
      break;

    case W_BISHOP:
    case B_BISHOP:
      generateBishopMoves(board, pos, legalMoves, onlyCaptures);
      break;

    case W_ROOK:
    case B_ROOK:
      generateRookMoves(board, pos, legalMoves, onlyCaptures);
      break;

    case W_QUEEN:
    case B_QUEEN:
      generateBishopMoves(board, pos, legalMoves, onlyCaptures);
      generateRookMoves(board, pos, legalMoves, onlyCaptures);
      break;

    case W_KING:
    case B_KING:
      generateKingMoves(board, pos, legalMoves, onlyCaptures);
      break;

    default:
      break;
  }
};

void MoveGenerator::generateKnightMoves(Board& board, Position pos, MoveList& moves, bool onlyCaptures) {
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

  Piece piece = board.getPiece(pos.file, pos.rank);

  for (const auto& jump : jumps) {
    int targetFile = pos.file + jump[0];
    int targetRank = pos.rank + jump[1];

    if (!board.isInside(targetFile, targetRank)) continue;

    Piece target = board.getPiece(targetFile, targetRank);

    if (target == EMPTY && !onlyCaptures) {
      tryMove(board, {pos, {targetFile, targetRank}, piece, target}, moves);
      continue;
    }

    if (board.getPieceColor(target) != board.getPieceColor(piece)) {
      tryMove(board, {pos, {targetFile, targetRank}, piece, target}, moves);
      continue;
    }
  }
}

void MoveGenerator::generateBishopMoves(Board& board, Position pos, MoveList& moves, bool onlyCaptures) {
  static const int directions[4][2] = {
    {-1, -1},
    {-1, 1},
    {1, -1},
    {1, 1},
  };

  Piece piece = board.getPiece(pos.file, pos.rank);

  for (const auto& direction : directions) {
    int currentFile = pos.file + direction[0];
    int currentRank = pos.rank + direction[1];

    while (board.isInside(currentFile, currentRank)) {
      Piece target = board.getPiece(currentFile, currentRank);

      if (target != EMPTY) {
        if (board.getPieceColor(target) != board.getPieceColor(piece)) {
          tryMove(board, {pos, {currentFile, currentRank}, piece, target}, moves);
        }
        break;
      }
      if (!onlyCaptures) tryMove(board, {pos, {currentFile, currentRank}, piece, target}, moves);

      currentFile += direction[0];
      currentRank += direction[1];
    }
  }
}

void MoveGenerator::generateRookMoves(Board& board, Position pos, MoveList& moves, bool onlyCaptures) {
  static const int directions[4][2] = {
    {0, -1},
    {0, 1},
    {1, 0},
    {-1, 0},
  };

  Piece piece = board.getPiece(pos.file, pos.rank);

  for (const auto& direction : directions) {
    int currentFile = pos.file + direction[0];
    int currentRank = pos.rank + direction[1];

    while (board.isInside(currentFile, currentRank)) {
      Piece target = board.getPiece(currentFile, currentRank);

      if (target != EMPTY) {
        if (board.getPieceColor(target) != board.getPieceColor(piece)) {
          tryMove(board, {pos, {currentFile, currentRank}, piece, target}, moves);
        }
        break;
      }          
      if (!onlyCaptures) tryMove(board, {pos, {currentFile, currentRank}, piece, target}, moves);

      currentFile += direction[0];
      currentRank += direction[1];
    }
  }
}

void MoveGenerator::generatePawnMoves(Board& board, Position pos, MoveList& moves, bool onlyCaptures) {
  Piece piece = board.getPiece(pos.file, pos.rank);

  bool isWhite = board.getPieceColor(piece) == WHITE;

  int direction = isWhite ? -1 : 1;
  int startRank = isWhite ? 6 : 1;
  int promotionRank = isWhite ? 1 : 6;

  int enPassantFile = board.enPassantFile;
  int enPassantRank = isWhite ? 2 : 5;

  static const int captureOffsets[2] = {-1, 1};

  int forwardRank = pos.rank + direction;

  if (forwardRank < 0 || forwardRank > 7) return;

  if (board.getPiece(pos.file, forwardRank) == EMPTY && !onlyCaptures) {
    if (pos.rank == promotionRank) {
      tryMove(board, {pos, {pos.file, forwardRank}, piece, EMPTY, PROMOTION, isWhite ? W_QUEEN : B_QUEEN}, moves);
      tryMove(board, {pos, {pos.file, forwardRank}, piece, EMPTY, PROMOTION, isWhite ? W_ROOK : B_ROOK}, moves);
      tryMove(board, {pos, {pos.file, forwardRank}, piece, EMPTY, PROMOTION, isWhite ? W_BISHOP : B_BISHOP}, moves);
      tryMove(board, {pos, {pos.file, forwardRank}, piece, EMPTY, PROMOTION, isWhite ? W_KNIGHT : B_KNIGHT}, moves);
    } else {
      tryMove(board, {pos, {pos.file, forwardRank}, piece, EMPTY}, moves);
    }

    if (pos.rank == startRank) {
      int doubleForwardRank = pos.rank + 2 * direction;

      if (board.getPiece(pos.file, doubleForwardRank) == EMPTY) {
        tryMove(board, {pos, {pos.file, doubleForwardRank}, piece, EMPTY}, moves);
      }
    }
  }

  for (const auto& offset : captureOffsets) {
    int targetFile = pos.file + offset;
    int targetRank = pos.rank + direction;

    if (!board.isInside(targetFile, targetRank)) continue;

    Piece capturablePiece = board.getPiece(targetFile, targetRank);

    if (capturablePiece == EMPTY) {
      if (targetFile == enPassantFile && targetRank == enPassantRank) {
        tryMove(board, {pos, {pos.file + offset, pos.rank + direction}, piece, isWhite ? B_PAWN : W_PAWN, EN_PASSANT}, moves);
      }
      continue;
    }

    if (board.getPieceColor(piece) != board.getPieceColor(capturablePiece)) {
      if (pos.rank == promotionRank) {
        tryMove(board, {pos, {targetFile, targetRank}, piece, capturablePiece, PROMOTION, isWhite ? W_QUEEN : B_QUEEN}, moves);
        tryMove(board, {pos, {targetFile, targetRank}, piece, capturablePiece, PROMOTION, isWhite ? W_ROOK : B_ROOK}, moves);
        tryMove(board, {pos, {targetFile, targetRank}, piece, capturablePiece, PROMOTION, isWhite ? W_BISHOP : B_BISHOP}, moves);
        tryMove(board, {pos, {targetFile, targetRank}, piece, capturablePiece, PROMOTION, isWhite ? W_KNIGHT : B_KNIGHT}, moves);
      } else {
        tryMove(board, {pos, {targetFile, targetRank}, piece, capturablePiece}, moves);
      }
    }
  }
}

void MoveGenerator::generateKingMoves(Board& board, Position pos, MoveList& moves, bool onlyCaptures) {
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

  Piece piece = board.getPiece(pos.file, pos.rank);

  for (const auto& direction : directions) {
    int currentFile = pos.file + direction[0];
    int currentRank = pos.rank + direction[1];

    if (!board.isInside(currentFile, currentRank)) continue;

    Piece target = board.getPiece(currentFile, currentRank);

    if (target == EMPTY && !onlyCaptures) {
      tryMove(board, {pos, {currentFile, currentRank}, piece, target}, moves);
    } else if (board.getPieceColor(target) != board.getPieceColor(piece)) {
      tryMove(board, {pos, {currentFile, currentRank}, piece, target}, moves);
    }
  }

  if (onlyCaptures) return;

  if (piece == W_KING && pos.file == 4 && pos.rank == 7) {
    if (board.castlingRights.whiteKingside) {
      bool isPathEmpty = board.getPiece(5, 7) == EMPTY && board.getPiece(6, 7) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked({4, 7}, WHITE) && !board.isSquareAttacked({5, 7}, WHITE) && !board.isSquareAttacked({6, 7}, WHITE);
      bool isRookThere = board.getPiece(7, 7) == W_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) tryMove(board, {pos, {6, 7}, piece, EMPTY, CASTLE_KINGSIDE}, moves); 
    }

    if (board.castlingRights.whiteQueenside) {
      bool isPathEmpty = board.getPiece(1, 7) == EMPTY && board.getPiece(2, 7) == EMPTY && board.getPiece(3, 7) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked({2, 7}, WHITE) && !board.isSquareAttacked({3, 7}, WHITE) && !board.isSquareAttacked({4, 7}, WHITE);
      bool isRookThere = board.getPiece(0, 7) == W_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) tryMove(board, {pos, {2, 7}, piece, EMPTY, CASTLE_QUEENSIDE}, moves); 
    }
  }

  if (piece == B_KING && pos.file == 4 && pos.rank == 0) {
    if (board.castlingRights.blackKingside) {
      bool isPathEmpty = board.getPiece(5, 0) == EMPTY && board.getPiece(6, 0) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked({4, 0}, BLACK) && !board.isSquareAttacked({5, 0}, BLACK) && !board.isSquareAttacked({6, 0}, BLACK);
      bool isRookThere = board.getPiece(7, 0) == B_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) tryMove(board, {pos, {6, 0}, piece, EMPTY, CASTLE_KINGSIDE}, moves); 
    }

    if (board.castlingRights.blackQueenside) {
      bool isPathEmpty = board.getPiece(1, 0) == EMPTY && board.getPiece(2, 0) == EMPTY && board.getPiece(3, 0) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked({2, 0}, BLACK) && !board.isSquareAttacked({3, 0}, BLACK) && !board.isSquareAttacked({4, 0}, BLACK);
      bool isRookThere = board.getPiece(0, 0) == B_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) tryMove(board, {pos, {2, 0}, piece, EMPTY, CASTLE_QUEENSIDE}, moves); 
    }
  }
}