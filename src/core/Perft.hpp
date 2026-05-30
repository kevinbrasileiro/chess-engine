#pragma once

#include <string>
#include "Board.hpp"

using U64 = unsigned long long;

U64 perft(Board& board, int depth, bool root);
