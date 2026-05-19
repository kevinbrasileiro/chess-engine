#pragma once

#include "Types.hpp"

struct Move {
    Position from;
    Position to;

    Piece movedPiece;
    Piece capturedPiece = EMPTY;
};