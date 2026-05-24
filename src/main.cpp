#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "core/Board.hpp"
#include "core/MoveGenerator.hpp"
#include "core/Perft.hpp"
#include "core/GameController.hpp"

const int TILE_SIZE = 100;
const int BOARD_SIZE = 8;

int main(int argc, char* argv[]) {
  sf::RenderWindow window(sf::VideoMode(TILE_SIZE * BOARD_SIZE, TILE_SIZE * BOARD_SIZE), "Chess");

  Board board;
  GameController controller(board);

  if (argc == 3 && std::string(argv[1]) == "--perft") {
    int depth = std::stoi(argv[2]);

    auto start = std::chrono::steady_clock::now();
    perft(board, depth, true);
    auto end = std::chrono::steady_clock::now();


    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Took: " << duration.count() << "ms" << std::endl;

    return 0;
  }

  if (argc == 3 && std::string(argv[1]) == "--bot") {
    std::string side = argv[2];

    controller.enableBot(side == "white" ? WHITE : BLACK);
  }

  std::map<Piece, sf::Texture> textures;

  textures[W_PAWN].loadFromFile("../assets/w-pawn.png");
  textures[W_ROOK].loadFromFile("../assets/w-rook.png");
  textures[W_KNIGHT].loadFromFile("../assets/w-knight.png");
  textures[W_BISHOP].loadFromFile("../assets/w-bishop.png");
  textures[W_QUEEN].loadFromFile("../assets/w-queen.png");
  textures[W_KING].loadFromFile("../assets/w-king.png");

  textures[B_PAWN].loadFromFile("../assets/b-pawn.png");
  textures[B_ROOK].loadFromFile("../assets/b-rook.png");
  textures[B_KNIGHT].loadFromFile("../assets/b-knight.png");
  textures[B_BISHOP].loadFromFile("../assets/b-bishop.png");
  textures[B_QUEEN].loadFromFile("../assets/b-queen.png");
  textures[B_KING].loadFromFile("../assets/b-king.png");

  for (auto& [piece, texture] : textures) {
    texture.setSmooth(true);
  }

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (event.type != sf::Event::MouseButtonPressed || event.mouseButton.button != sf::Mouse::Left) continue;

      Position clickedPos = {event.mouseButton.x / TILE_SIZE, event.mouseButton.y / TILE_SIZE};
      controller.handleClick(clickedPos);
    }

    window.clear();

    for (int file = 0; file < BOARD_SIZE; file++) {
      for (int rank = 0; rank < BOARD_SIZE; rank++) {
        sf::RectangleShape square(sf::Vector2f(TILE_SIZE, TILE_SIZE));

        square.setPosition(file * TILE_SIZE, rank * TILE_SIZE);

        if ((file + rank) % 2 == 0) {
          square.setFillColor(sf::Color(240, 217, 181));
        } else {
          square.setFillColor(sf::Color(181, 136, 99));
        }

        if (controller.isSelected({file, rank})) {
          square.setFillColor(sf::Color(240, 238, 132));
        }

        window.draw(square);

        Piece piece = board.getPiece(file, rank);

        if (piece == EMPTY) continue;

        sf::Sprite sprite;

        sprite.setTexture(textures[piece]);

        sprite.setScale(90.f / 128.f, 90.f / 128.f);
        sprite.setPosition(file * TILE_SIZE + 5, rank * TILE_SIZE + 5);

        window.draw(sprite);
      }
    }

    window.display();
  }

  return 0;
}