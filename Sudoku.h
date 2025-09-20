//
// Created by zwidofhelangani on 2025/09/14.
//

#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H

#include "Button.h"
#include <vector>
#include <string>

class Sudoku {
private:
    int level {1};
    int errors{0};

    sf::Font font;
    std::vector<sf::Text> texts;
    sf::Text line;
    std::vector<std::vector<int>> board;
    std::vector<std::vector<int>> boardValues;
    std::vector<std::vector<sf::RectangleShape>>blocks;

    void generateBoard(); // Fills boardValues with the complete sudoku board values
    bool fillGrid();
    bool valid(int row, int col, int value) const;
    void generateWindowBoard(sf::RenderWindow& parentWindow);

    bool correctNum(int row, int col, int val);
    bool userInput(const sf::RenderWindow& window);
    void resetBoard();

    class FailedToLoadFont{};
public:
    Sudoku();
    ~Sudoku() = default;
    void printBoard(sf::RenderWindow& mainW); //Prints the sudoku board as an interactive window
};

#endif //SUDOKU_SUDOKU_H
