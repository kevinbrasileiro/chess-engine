#include <iostream>
#include "MoveGenerator.hpp"

std::vector<Move> MoveGenerator::generateMoves(Board& board, Position pos) {
  std::vector<Move> legalMoves;
  std::vector<Move> pseudoMoves = generatePseudoLegalMoves(board, pos);

  Color movingSide = board.getTurn();
  for (const auto& move : pseudoMoves) {
    board.makeMove(move);
    Position kingPos = board.findKing(movingSide);

    if (!board.isSquareAttacked(kingPos, movingSide)) {
      legalMoves.push_back(move);
    }

    board.undoMove(move);
  }

  return legalMoves;
}

std::vector<Move> MoveGenerator::generatePseudoLegalMoves(const Board& board, Position pos) {
  std::vector<Move> moves = {};
  Piece piece = board.getPiece(pos);

  switch (piece) {
    case W_PAWN:
    case B_PAWN:
      generatePawnMoves(board, pos, moves);
      break;

    case W_KNIGHT:
    case B_KNIGHT:
      generateKnightMoves(board, pos, moves);
      break;

    case W_BISHOP:
    case B_BISHOP:
      generateBishopMoves(board, pos, moves);
      break;

    case W_ROOK:
    case B_ROOK:
      generateRookMoves(board, pos, moves);
      break;

    case W_QUEEN:
    case B_QUEEN:
      generateBishopMoves(board, pos, moves);
      generateRookMoves(board, pos, moves);
      break;

    case W_KING:
    case B_KING:
      generateKingMoves(board, pos, moves);
      break;

    default:
      break;
  }

  return moves;
};

void MoveGenerator::generateKnightMoves(const Board& board, Position pos, std::vector<Move>& moves) {
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

  Piece piece = board.getPiece(pos);

  for (const auto& jump : jumps) {
    int targetFile = pos.file + jump[0];
    int targetRank = pos.rank + jump[1];

    if (!board.isInside(targetFile, targetRank)) continue;

    Piece target = board.getPiece({targetFile, targetRank});

    if (target == EMPTY) {
      moves.push_back({pos, {targetFile, targetRank}, piece, target});
      continue;
    }

    if (board.getPieceColor(target) != board.getPieceColor(piece)) {
      moves.push_back({pos, {targetFile, targetRank}, piece, target});
      continue;
    }
  }
}

void MoveGenerator::generateBishopMoves(const Board& board, Position pos, std::vector<Move>& moves) {
  static const int directions[4][2] = {
    {-1, -1},
    {-1, 1},
    {1, -1},
    {1, 1},
  };

  Piece piece = board.getPiece(pos);

  for (const auto& direction : directions) {
    int currentFile = pos.file + direction[0];
    int currentRank = pos.rank + direction[1];

    while (board.isInside(currentFile, currentRank)) {
      Piece target = board.getPiece({currentFile, currentRank});

      if (target == EMPTY) {
        moves.push_back({pos, {currentFile, currentRank}, piece, target});
      } else {

        if (board.getPieceColor(target) != board.getPieceColor(piece)) {
          moves.push_back({pos, {currentFile, currentRank}, piece, target});
        }

        break;
      }

      currentFile += direction[0];
      currentRank += direction[1];
    }
  }
}

void MoveGenerator::generateRookMoves(const Board& board, Position pos, std::vector<Move>& moves) {
  static const int directions[4][2] = {
    {0, -1},
    {0, 1},
    {1, 0},
    {-1, 0},
  };

  Piece piece = board.getPiece(pos);

  for (const auto& direction : directions) {
    int currentFile = pos.file + direction[0];
    int currentRank = pos.rank + direction[1];

    while (board.isInside(currentFile, currentRank)) {
      Piece target = board.getPiece({currentFile, currentRank});

      if (target == EMPTY) {
        moves.push_back({pos, {currentFile, currentRank}, piece, target});
      } else {

        if (board.getPieceColor(target) != board.getPieceColor(piece)) {
          moves.push_back({pos, {currentFile, currentRank}, piece, target});
        }

        break;
      }

      currentFile += direction[0];
      currentRank += direction[1];
    }
  }
}

void MoveGenerator::generatePawnMoves(const Board& board, Position pos, std::vector<Move>& moves) {
  Piece piece = board.getPiece(pos);

  bool isWhite = board.getPieceColor(piece) == WHITE;

  int direction = isWhite ? -1 : 1;
  int startRank = isWhite ? 6 : 1;
  int promotionRank = isWhite ? 1 : 6;

  int enPassantFile = board.enPassantFile;
  int enPassantRank = isWhite ? 2 : 5;

  static const int captureOffsets[2] = {-1, 1};

  int forwardRank = pos.rank + direction;

  if (forwardRank < 0 || forwardRank > 7) return;

  if (board.getPiece({pos.file, forwardRank}) == EMPTY) {
    if (pos.rank == promotionRank) {
      moves.push_back({pos, {pos.file, forwardRank}, piece, EMPTY, PROMOTION_QUEEN});
      moves.push_back({pos, {pos.file, forwardRank}, piece, EMPTY, PROMOTION_ROOK});
      moves.push_back({pos, {pos.file, forwardRank}, piece, EMPTY, PROMOTION_BISHOP});
      moves.push_back({pos, {pos.file, forwardRank}, piece, EMPTY, PROMOTION_KNIGHT});
    } else {
        moves.push_back({pos, {pos.file, forwardRank}, piece, EMPTY});
    }

    if (pos.rank == startRank) {
      int doubleForwardRank = pos.rank + 2 * direction;

      if (board.getPiece({pos.file, doubleForwardRank}) == EMPTY) {
        moves.push_back({pos, {pos.file, doubleForwardRank}, piece, EMPTY});
      }
    }
  }

  for (const auto& offset : captureOffsets) {
    int targetFile = pos.file + offset;
    int targetRank = pos.rank + direction;

    if (!board.isInside(targetFile, targetRank)) continue;

    Piece capturablePiece = board.getPiece({targetFile, targetRank});

    if (capturablePiece == EMPTY) {
      if (targetFile == enPassantFile && targetRank == enPassantRank) {
        moves.push_back({pos, {pos.file + offset, pos.rank + direction}, piece, isWhite ? B_PAWN : W_PAWN, EN_PASSANT});
      }
      continue;
    }

    if (board.getPieceColor(piece) != board.getPieceColor(capturablePiece)) {
      if (pos.rank == promotionRank) {
        moves.push_back({pos, {targetFile, targetRank}, piece, capturablePiece, PROMOTION_QUEEN});
        moves.push_back({pos, {targetFile, targetRank}, piece, capturablePiece, PROMOTION_ROOK});
        moves.push_back({pos, {targetFile, targetRank}, piece, capturablePiece, PROMOTION_BISHOP});
        moves.push_back({pos, {targetFile, targetRank}, piece, capturablePiece, PROMOTION_KNIGHT});
      } else {
        moves.push_back({pos, {targetFile, targetRank}, piece, capturablePiece});
      }
    }
  }
}

void MoveGenerator::generateKingMoves(const Board& board, Position pos, std::vector<Move>& moves) {
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

  Piece piece = board.getPiece(pos);

  for (const auto& direction : directions) {
    int currentFile = pos.file + direction[0];
    int currentRank = pos.rank + direction[1];

    if (!board.isInside(currentFile, currentRank)) continue;

    Piece target = board.getPiece({currentFile, currentRank});

    if (target == EMPTY) {
      moves.push_back({pos, {currentFile, currentRank}, piece, target});
    } else if (board.getPieceColor(target) != board.getPieceColor(piece)) {
      moves.push_back({pos, {currentFile, currentRank}, piece, target});
    }
  }

  if (piece == W_KING && pos.file == 4 && pos.rank == 7) {
    if (board.castlingRights.whiteKingside) {
      bool isPathEmpty = board.getPiece({5, 7}) == EMPTY && board.getPiece({6, 7}) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked({4, 7}, WHITE) && !board.isSquareAttacked({5, 7}, WHITE) && !board.isSquareAttacked({6, 7}, WHITE);
      bool isRookThere = board.getPiece({7, 7}) == W_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) moves.push_back({pos, {6, 7}, piece, EMPTY, CASTLE_KINGSIDE}); 
    }

    if (board.castlingRights.whiteQueenside) {
      bool isPathEmpty = board.getPiece({1, 7}) == EMPTY && board.getPiece({2, 7}) == EMPTY && board.getPiece({3, 7}) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked({2, 7}, WHITE) && !board.isSquareAttacked({3, 7}, WHITE) && !board.isSquareAttacked({4, 7}, WHITE);
      bool isRookThere = board.getPiece({0, 7}) == W_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) moves.push_back({pos, {2, 7}, piece, EMPTY, CASTLE_QUEENSIDE}); 
    }
  }

  if (piece == B_KING && pos.file == 4 && pos.rank == 0) {
    if (board.castlingRights.blackKingside) {
      bool isPathEmpty = board.getPiece({5, 0}) == EMPTY && board.getPiece({6, 0}) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked({4, 0}, BLACK) && !board.isSquareAttacked({5, 0}, BLACK) && !board.isSquareAttacked({6, 0}, BLACK);
      bool isRookThere = board.getPiece({7, 0}) == B_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) moves.push_back({pos, {6, 0}, piece, EMPTY, CASTLE_KINGSIDE}); 
    }

    if (board.castlingRights.blackQueenside) {
      bool isPathEmpty = board.getPiece({1, 0}) == EMPTY && board.getPiece({2, 0}) == EMPTY && board.getPiece({3, 0}) == EMPTY;
      bool isPathSafe = !board.isSquareAttacked({2, 0}, BLACK) && !board.isSquareAttacked({3, 0}, BLACK) && !board.isSquareAttacked({4, 0}, BLACK);
      bool isRookThere = board.getPiece({0, 0}) == B_ROOK;

      if (isPathEmpty && isPathSafe && isRookThere) moves.push_back({pos, {2, 0}, piece, EMPTY, CASTLE_QUEENSIDE}); 
    }
  }
  
}