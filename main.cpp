//#include <iostream>
#include "Button.h"
#include "Sudoku.h"
#include "ScoreBoard.h"
#include <filesystem>

int main() {
    unsigned int mainWindowHeight =  800;
    unsigned int mainWindowWidth =  800;
    sf::RenderWindow mainWindow(sf::VideoMode{mainWindowWidth, mainWindowHeight}, "Sudoku Homepage");

    std::filesystem::create_directories("GameFiles");
    Sudoku sudoku;

    //Exit Button
    Button exit("Textures/exit.png");
    exit.setScale({0.05f, 0.05f});
    sf::FloatRect exitBounds = exit.getSprite().getGlobalBounds();
    float exitX = exitBounds.width;
    sf::Vector2f exitPos = {static_cast<float>(mainWindowWidth/2) - exitX/2.0f, static_cast<float>(mainWindowWidth/1.25f)};
    exit.setPosition(exitPos);

    //Start
    Button start("Textures/start.png");
    start.setScale({0.275f, 0.275f});
    sf::FloatRect startBounds = start.getSprite().getGlobalBounds();
    float startX = startBounds.width;
    sf::Vector2f startPos = {static_cast<float>(mainWindowWidth/2) - startX/2.0f, static_cast<float>(mainWindowWidth/2.0f)};
    start.setPosition(startPos);

    //Scoreboard button
    Button score("Textures/score.png");
    score.setScale({0.075f, 0.075f});
    sf::FloatRect scoreBounds = score.getSprite().getGlobalBounds();
    float scoreX = scoreBounds.width;
    sf::Vector2f scorePos = {static_cast<float>(mainWindowWidth/2) - scoreX/2.0f, static_cast<float>(mainWindowWidth/1.55f)};
    score.setPosition(scorePos);

    mainWindow.setFramerateLimit(60);

    while (mainWindow.isOpen()) {
        sf::Event event;
        while (mainWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||exit.selected(mainWindow))
                mainWindow.close();
            else if (event.type == sf::Event::Resized) {
                exit.resize(mainWindowHeight, mainWindowWidth, mainWindow);
                start.resize(mainWindowHeight, mainWindowWidth, mainWindow);
                score.resize(mainWindowHeight, mainWindowWidth, mainWindow);
            }
            //Opens new window if start button was pressed
            else if (start.selected(mainWindow)) {
                sudoku.setLevel(getLevelFromFile("GameFiles/level.txt"));
                mainWindow.setVisible(false);
                sudoku.printBoard(mainWindow);
                saveRecordsToFile(sudoku.getLevel(), sudoku.getTime(), sudoku.getStatus(), "GameFiles/records.txt");
                saveLevelToFile("GameFiles/level.txt", sudoku.getLevel());
            }
            else if (score.selected(mainWindow)) {
                mainWindow.setVisible(false);
                scoreBoard(mainWindow, sudoku);
            }
        }

        //Update

        //Draws
        mainWindow.clear(sf::Color::White);
        mainWindow.draw(exit.getSprite());
        mainWindow.draw(start.getSprite());
        mainWindow.draw(score.getSprite());
        mainWindow.display();
    }

    return 0;
}