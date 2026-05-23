#pragma once

struct Position {
    int file;
    int rank;
};

struct CastlingRights {
    bool whiteKingside = true;
    bool whiteQueenside = true;
    bool blackKingside = true;
    bool blackQueenside = true;
};

enum Piece {
    EMPTY,

    W_PAWN,
    W_ROOK,
    W_KNIGHT,
    W_BISHOP,
    W_QUEEN,
    W_KING,

    B_PAWN,
    B_ROOK,
    B_KNIGHT,
    B_BISHOP,
    B_QUEEN,
    B_KING,
};

enum Color {
    NO_COLOR,
    WHITE,
    BLACK
};