cmake_minimum_required(VERSION 3.16)

project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(ChessEngine
    src/main.cpp
    src/core/Board.cpp
    src/core/MoveGenerator.cpp
    src/core/GameController.cpp
    src/core/Perft.cpp
    src/core/MoveTables.cpp

    src/bot/Evaluation.cpp
    src/bot/Search.cpp
)

target_link_libraries(ChessEngine
    sfml-graphics
    sfml-window
    sfml-system
)

target_compile_options(ChessEngine PRIVATE 
    $<$<CONFIG:Release>:-march=native> 
    $<$<CONFIG:RelWithDebInfo>:-O2>
)