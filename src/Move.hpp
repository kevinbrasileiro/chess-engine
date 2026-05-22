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
    Position from;
    Position to;

    Piece movedPiece;
    Piece capturedPiece = EMPTY;

    MoveFlag flag = NORMAL;
};