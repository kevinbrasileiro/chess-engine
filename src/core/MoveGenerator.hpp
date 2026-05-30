#pragma once

#include "Board.hpp"
#include "MoveList.hpp"
#include "MoveTables.hpp"

class MoveGenerator {
public:
  static void generateAllMoves(Board& board, MoveList& allMoves, Color side, bool onlyCaptures);
  static void generatePieceMoves(Board& board, int square, MoveList& legalMoves, bool onlyCaptures);
  private:
  inline static void tryMove(Board& board, const Move& move, MoveList& legalMoves) {
    Color side = board.getTurn();

    board.makeMove(move);
    
    if (!board.isSquareAttacked(board.findKing(side), side)) {
      legalMoves.add(move);
    }

    board.undoMove(move);
  };
  static void generatePawnMoves(Board& board, int square, MoveList& moves, bool onlyCaptures);
  static void generateKnightMoves(Board& board, int square, MoveList& moves, bool onlyCaptures);
  static void generateBishopMoves(Board& board, int square, MoveList& moves, bool onlyCaptures);
  static void generateRookMoves(Board& board, int square, MoveList& moves, bool onlyCaptures);
  static void generateKingMoves(Board& board, int square, MoveList& moves, bool onlyCaptures);
};