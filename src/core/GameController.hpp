#pragma once

#include "../core/Board.hpp"

class GameController {
public:
  GameController(Board& board);

  void handleClick(Position clickedPos);

  bool isSelected(Position pos) const;

  void enableBot(Color color);

private:
  Board& board;

  bool selected = false;
  Position selectedPos = {-1, -1};

  bool botEnabled = false;
  Color botColor = BLACK;
  void makeBotMove();
};