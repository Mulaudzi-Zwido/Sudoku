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
    std::string** board;
    std::string** boardValues;
    std::vector<sf::RectangleShape> blocks;
public:
    Sudoku();
    ~Sudoku();
    void printBoard(sf::RenderWindow& mainW);
};

#endif //SUDOKU_SUDOKU_H