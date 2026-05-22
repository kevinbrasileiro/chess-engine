cmake_minimum_required(VERSION 3.16)

project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(ChessEngine
    src/main.cpp
    src/Board.cpp
    src/MoveGenerator.cpp
    src/Perft.cpp
)

target_link_libraries(ChessEngine
    sfml-graphics
    sfml-window
    sfml-system
)
