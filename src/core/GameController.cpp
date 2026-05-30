#include <iostream>
#include <random>

#include "GameController.hpp"
#include "MoveGenerator.hpp"
#include "../bot/Search.hpp"

GameController::GameController(Board& board): board(board) {}

void GameController::handleClick(Position clickedPos) {
  if (botEnabled && board.getTurn() == botColor) return;

  Piece clickedPiece = board.getPiece(clickedPos.file, clickedPos.rank);
  Color clickedPieceColor = board.getPieceColor(clickedPiece);

  MoveList availableMoves;

  if (selected) {
    MoveGenerator::generatePieceMoves(board, selectedPos, availableMoves, false);
    bool moved = false;

    for (int i = 0; i < availableMoves.count; i++) {
      const Move& move = availableMoves[i];
      if (move.to.file == clickedPos.file && move.to.rank == clickedPos.rank) {
        board.makeMove(move);
        moved = true;

        selected = false;
        selectedPos = {-1, -1};

        if (botEnabled) makeBotMove();
        break;
      } 
    }

    if (moved || clickedPiece == EMPTY) {
      selected = false;
      selectedPos = {-1, -1};
    } else if (clickedPieceColor == board.getTurn()) {
      selected = true;
      selectedPos = clickedPos;
    }

  } else if (clickedPiece != EMPTY && clickedPieceColor == board.getTurn()) {
    selected = true;
    selectedPos = clickedPos;
  }
}

bool GameController::isSelected(Position pos) const {
  return pos.file == selectedPos.file && pos.rank == selectedPos.rank;
}

void GameController::enableBot(Color color) {
  botEnabled = true;
  botColor = color;

  if (board.getTurn() == botColor) makeBotMove();
}

void GameController::makeBotMove() {
  MoveList moves;
  moves.clear();
  
  MoveGenerator::generateAllMoves(board, moves, botColor, false);

  if (moves.count == 0) return;

  int bestEval = -1000000;
  int bestMove = 0;

  for (int i = 0; i < moves.count; i++) {
      const Move& move = moves[i];

      board.makeMove(move);

      int eval = -Search::searchPosition(board, 4, -1000000, 1000000);

      if (eval > bestEval) {
        bestEval = eval;
        bestMove = i;
      }

      board.undoMove(move);
  }

  board.makeMove(moves[bestMove]);
}