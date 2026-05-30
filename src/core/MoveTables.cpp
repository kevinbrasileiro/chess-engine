#include "MoveTables.hpp"

namespace MoveTables {
  SquareList pawnAttacks[2][64];
  SquareList knightMoves[64];
  Ray bishopRays[64][4];
  Ray rookRays[64][4];
  SquareList kingMoves[64];

  void initRays() {
    constexpr int bishopDirs[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    constexpr int rookDirs[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    for (int sq = 0; sq < 64; sq++) {
      int rank = sq / 8;
      int file = sq % 8;

      for (int dir = 0; dir < 4; dir++) {
        bishopRays[sq][dir].count = 0;

        int r = rank + bishopDirs[dir][0];
        int f = file + bishopDirs[dir][1];

        while (r >= 0 && r < 8 && f >= 0 && f < 8) {
          int targetSq = r * 8 + f;
          bishopRays[sq][dir].squares[bishopRays[sq][dir].count++] = targetSq;

          r += bishopDirs[dir][0];
          f += bishopDirs[dir][1];
        }
      }

      for (int dir = 0; dir < 4; dir++) {
        rookRays[sq][dir].count = 0;

        int r = rank + rookDirs[dir][0];
        int f = file + rookDirs[dir][1];

        while (r >= 0 && r < 8 && f >= 0 && f < 8) {
          int targetSq = r * 8 + f;
          rookRays[sq][dir].squares[rookRays[sq][dir].count++] = targetSq;

          r += rookDirs[dir][0];
          f += rookDirs[dir][1];
        }
      }
    }
  }

  void initStepMoves() {
    constexpr int knightOffsets[8][2] = {
      {2, 1}, {2, -1},
      {-2, 1}, {-2, -1},
      {1, 2}, {1, -2},
      {-1, 2}, {-1, -2}
    };

    constexpr int kingOffsets[8][2] = {
      {1, 1}, {1, 0}, {1, -1},
      {0, 1},          {0, -1},
      {-1, 1}, {-1, 0}, {-1, -1}
    };

    for (int sq = 0; sq < 64; sq++) {
      int rank = sq / 8;
      int file = sq % 8;

      knightMoves[sq].count = 0;
      kingMoves[sq].count = 0;

      for (auto [dr, df] : knightOffsets) {
        int r = rank + dr;
        int f = file + df;

        if (r < 0 || r > 7 || f < 0 || f > 7) continue;

        knightMoves[sq].squares[knightMoves[sq].count++] = r * 8 + f;
      }

      for (auto [dr, df] : kingOffsets) {
        int r = rank + dr;
        int f = file + df;

        if (r < 0 || r > 7 || f < 0 || f > 7) continue;

        kingMoves[sq].squares[kingMoves[sq].count++] = r * 8 + f;
      }

      pawnAttacks[0][sq].count = 0;
      pawnAttacks[1][sq].count = 0;

      int wr = rank - 1;
      if (wr >= 0) {
        if (file > 0) pawnAttacks[0][sq].squares[pawnAttacks[0][sq].count++] = wr * 8 + (file - 1);
        if (file < 7) pawnAttacks[0][sq].squares[pawnAttacks[0][sq].count++] = wr * 8 + (file + 1);
      }

      int br = rank + 1;
      if (br <= 7) {
        if (file > 0) pawnAttacks[1][sq].squares[pawnAttacks[1][sq].count++] = br * 8 + (file - 1);
        if (file < 7) pawnAttacks[1][sq].squares[pawnAttacks[1][sq].count++] = br * 8 + (file + 1);
      }
    }
  }

  void init() {
    initStepMoves();
    initRays();
  }
}