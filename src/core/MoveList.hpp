#include "Move.hpp"

struct MoveList {
  Move moves[256];
  int count = 0;

  inline void add(const Move& move) {
    if (count > 256) return;
    moves[count++] = move;
  }

  inline void clear() {
    count = 0;
  }

  inline const Move& operator[](int i) const {
    return moves[i];
  }
};