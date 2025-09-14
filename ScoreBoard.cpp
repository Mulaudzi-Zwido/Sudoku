//
// Created by zwidofhelangani on 2025/09/14.
//

#include "ScoreBoard.h"

void scoreBoard (sf::RenderWindow& window) {
    sf::RenderWindow scoreWindow(sf::VideoMode{800, 800}, "Sudoku Scoreboard");

    scoreWindow.setFramerateLimit(60);

    while (scoreWindow.isOpen()) {
        sf::Event gameEvent;
        while (scoreWindow.pollEvent(gameEvent)) {
            if (gameEvent.type == sf::Event::Closed) {
                window.setVisible(true);
                scoreWindow.close();
            }
        }
        scoreWindow.clear(sf::Color::White);
        scoreWindow.display();
    }
}