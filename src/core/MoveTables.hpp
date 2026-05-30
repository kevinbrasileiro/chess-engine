#pragma once

#include <cstdint>

struct SquareList {
    uint8_t count;
    uint8_t squares[8];
};

struct Ray {
    uint8_t count;
    uint8_t squares[7];
};

namespace MoveTables {
    extern SquareList pawnAttacks[2][64]; // 0-white / 1-black
    
    extern SquareList knightMoves[64];

    extern Ray bishopRays[64][4];
    extern Ray rookRays[64][4];

    extern SquareList kingMoves[64];

    void init();
}