#pragma once

#include <string>
#include "Board.hpp"

using U64 = unsigned long long;

std::string squareToString(Position pos);
std::string moveToString(const Move& move);

U64 perft(Board& board, int depth, bool root);
