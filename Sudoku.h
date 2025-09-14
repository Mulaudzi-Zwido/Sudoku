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
    int ** board;
    int** boardValues;
    std::vector<sf::RectangleShape> blocks;
public:
    Sudoku();
    ~Sudoku();
    void fillBoardValues(); // Fills boardValues with the complete sudoku board values
    void reinitializeRow(int row);
    bool rowDuplicates(int column, int value) const;
    bool colDuplicates(int row, int value) const;
    bool boxDuplicates(int row, int col, int value) const;
    void fillIncompleteBoard();
    void printBoard(sf::RenderWindow& mainW); //Prints the sudoku board as an interactive window
};

#endif //SUDOKU_SUDOKU_H