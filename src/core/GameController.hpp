#pragma once

#include "../core/Board.hpp"

class GameController {
public:
  GameController(Board& board);

  void handleClick(int clickedSquare);

  bool isSelected(int square) const;

  void enableBot(Color color);

  inline bool isBotMovePending() const {return botMovePending;}
  void makeBotMove();

private:
  Board& board;

  bool selected = false;
  int selectedSquare = -1;

  bool botEnabled = false;
  Color botColor = NO_COLOR;

  bool botMovePending = false;
};