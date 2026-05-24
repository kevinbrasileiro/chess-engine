#include <random>

#include "GameController.hpp"
#include "MoveGenerator.hpp"

GameController::GameController(Board& board): board(board) {}

void GameController::handleClick(Position clickedPos) {
  if (botEnabled && board.getTurn() == botColor) return;

  Piece clickedPiece = board.getPiece(clickedPos.file, clickedPos.rank);
  Color clickedPieceColor = board.getPieceColor(clickedPiece);

  MoveList availableMoves;

  if (selected) {
    MoveGenerator::generateMoves(board, selectedPos, availableMoves);
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
  MoveList availableMoves;

  for (int file = 0; file < 8; file++) {
    for (int rank = 0; rank < 8; rank++) {
      Position pos = {file, rank};
      Piece piece = board.getPiece(pos.file, pos.rank);

      if (piece == EMPTY) continue;
      if (board.getPieceColor(piece) != botColor) continue;
      
      MoveGenerator::generateMoves(board, {file, rank}, availableMoves);
    }
  }

  if (availableMoves.count == 0) return;

  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::uniform_int_distribution<> dist(0, availableMoves.count - 1);

  Move randomMove = availableMoves[dist(gen)];

  board.makeMove(randomMove);
}