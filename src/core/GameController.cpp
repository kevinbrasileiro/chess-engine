#include <iostream>
#include <random>

#include "GameController.hpp"
#include "MoveGenerator.hpp"
#include "../bot/Search.hpp"

GameController::GameController(Board& board): board(board) {}

void GameController::handleClick(int clickedSquare) {
  if (botEnabled && board.getTurn() == botColor) return;

  Piece clickedPiece = board.getPiece(clickedSquare);
  Color clickedPieceColor = board.getPieceColor(clickedPiece);

  MoveList availableMoves;

  if (selected) {
    MoveGenerator::generatePieceMoves(board, selectedSquare, availableMoves, false);
    bool moved = false;

    for (int i = 0; i < availableMoves.count; i++) {
      const Move& move = availableMoves[i];
      if (move.to == clickedSquare) {
        board.makeMove(move);
        moved = true;

        selected = false;
        selectedSquare = -1;

        if (botEnabled) makeBotMove();
        break;
      } 
    }

    if (moved || clickedPiece == EMPTY) {
      selected = false;
      selectedSquare = -1;
    } else if (clickedPieceColor == board.getTurn()) {
      selected = true;
      selectedSquare = clickedSquare;
    }

  } else if (clickedPiece != EMPTY && clickedPieceColor == board.getTurn()) {
    selected = true;
    selectedSquare = clickedSquare;
  }
}

bool GameController::isSelected(int square) const {
  return square == selectedSquare;
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