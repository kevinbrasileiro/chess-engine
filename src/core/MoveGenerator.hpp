#pragma once

#include "Board.hpp"
#include "MoveList.hpp"

class MoveGenerator {
public:
  static void generateAllMoves(Board& board, MoveList& allMoves, Color side);
  static void generateAllCaptures(Board& board, MoveList& allMoves, Color side);
  static void generatePieceMoves(Board& board, Position pos, MoveList& legalMoves);
  private:
  inline static void tryMove(Board& board, const Move& move, MoveList& legalMoves) {
    Color side = board.getTurn();

    board.makeMove(move);
    
    if (!board.isSquareAttacked(board.findKing(side), side)) {
      legalMoves.add(move);
    }

    board.undoMove(move);
  };
  static void generatePawnMoves(Board& board, Position pos, MoveList& moves);
  static void generateKnightMoves(Board& board, Position pos, MoveList& moves);
  static void generateBishopMoves(Board& board, Position pos, MoveList& moves);
  static void generateRookMoves(Board& board, Position pos, MoveList& moves);
  static void generateKingMoves(Board& board, Position pos, MoveList& moves);
};