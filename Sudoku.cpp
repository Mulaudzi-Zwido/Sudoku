//
// Created by zwidofhelangani on 2025/09/14.
//

#include "Sudoku.h"
#include <random>

Sudoku::Sudoku() {
    board = new std::string *[9];
    boardValues = new std::string *[9];

    for (int i = 0; i < 9; ++i) {
        board[i] = new std::string[9];
        boardValues[i] = new std::string[9];
    }

}

Sudoku::~Sudoku() {

    for (int i = 0; i < 9; ++i) {
        delete [] board[i];
        board[i] = nullptr;
        delete [] boardValues[i];
        boardValues[i] = nullptr;
    }

    delete [] board;
    board = nullptr;
    delete [] boardValues;
    boardValues = nullptr;
}


void Sudoku::printBoard(sf::RenderWindow &mainW) {
    sf::RenderWindow window(sf::VideoMode{800, 800}, "Sudoku");

    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event gameEvent;
        while (window.pollEvent(gameEvent)) {
            if (gameEvent.type == sf::Event::Closed) {
                mainW.setVisible(true);
                window.close();
            }
        }
        window.clear(sf::Color::White);
        window.display();
    }
}