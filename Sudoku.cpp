//
// Created by zwidofhelangani on 2025/09/14.
//

#include "Sudoku.h"

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