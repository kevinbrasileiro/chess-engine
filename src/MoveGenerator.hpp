#pragma once

#include <vector>

#include "Board.hpp"

class MoveGenerator {
public:
  static std::vector<Move> generateMoves(Board& board, Position pos);
  static std::vector<Move> generatePseudoLegalMoves(const Board& board, Position pos);
private:
  static void generatePawnMoves(const Board& board, Position pos, std::vector<Move>& moves);
  static void generateKnightMoves(const Board& board, Position pos, std::vector<Move>& moves);
  static void generateBishopMoves(const Board& board, Position pos, std::vector<Move>& moves);
  static void generateRookMoves(const Board& board, Position pos, std::vector<Move>& moves);
  static void generateKingMoves(const Board& board, Position pos, std::vector<Move>& moves);
};