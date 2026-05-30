#pragma once

#include "Types.hpp"

enum MoveFlag {
    NORMAL, 
    CASTLE_KINGSIDE,
    CASTLE_QUEENSIDE,
    EN_PASSANT,

    PROMOTION,
};

struct Move {
    int from;
    int to;

    Piece movedPiece;
    Piece capturedPiece = EMPTY;

    MoveFlag flag = NORMAL;
    Piece promotedPiece = EMPTY;
};