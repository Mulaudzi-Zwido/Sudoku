//
// Created by zwidofhelangani on 2025/09/14.
//

#include "Sudoku.h"
#include <random>

Sudoku::Sudoku() {
    board = new int *[9];
    boardValues = new int *[9];

    for (int i = 0; i < 9; ++i) {
        board[i] = new int[9];
        boardValues[i] = new int[9];
    }

    //Initialize board
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            board[i][j] = 0;
            boardValues[i][j] = 0;
        }
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

void Sudoku::reinitializeRow(int row) {
    for (int i = 0; i < 9; ++i)
        boardValues[row][i] = 0;
}

bool Sudoku::rowDuplicates(int column, int value) const{
    for (int i = 0; i < 9; ++i)
        if (boardValues[i][column] == value)
            return true;
    return false;
}

bool Sudoku::colDuplicates(int row, int value) const {
    for (int i = 0; i < 9; ++i)
        if (boardValues[row][i] == value)
            return true;
    return false;
}

bool Sudoku::boxDuplicates(int row, int col, int value) const {
    for (int i = row; i < row + 3; ++i) {
        for (int j = col; j < col + 3; ++j)
            if (boardValues[i][j] == value)
                return true;
    }
    return false;
}

void Sudoku::fillBoardValues() {
    //For getting a random number
    std::random_device rd;
    std::uniform_int_distribution<int> dist(1,9);

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int value = dist(rd);
            //If there is a duplicate go back to the previous row and restart from there
            //If it is still the first row restart from there

            //Row Duplicates
            if (rowDuplicates(j, value)) {
                reinitializeRow(i);
                if (i == 0) { //Checks if it's in the first row
                    i -= 1;
                }else {
                    reinitializeRow(--i);
                    i -= 2;
                }
                break;
            }

            //Column Duplicates
            if (colDuplicates(i, value)) {
                reinitializeRow(i);
                if (i == 0) { //Checks if it's in the first row
                    i -= 1;
                }else {
                    reinitializeRow(--i);
                    i -= 2;
                }
                break;
            }

            if (i%3 == 0 && j%3 == 0) { //Checks if it's an appropriate place to look for box duplicates
                if (boxDuplicates(i, j, value)) {
                    reinitializeRow(i);
                    if (i == 0) { //Checks if it's in the first row
                        i -= 1;
                    }else {
                        reinitializeRow(--i);
                        i -= 2;
                    }
                    break;
                }
            }

            boardValues[i][j] = value; // If there are no duplicates insert the value
        }
    }
}

void Sudoku::fillIncompleteBoard() {
    std::random_device rd;
    int min, max;

    if (level <= 10) {
        min = 45;
        max = 49;
    } else if (level > 10 && level <= 20) {
        min = 40;
        max = 44;
    } else if (level > 20 && level <= 30) {
        min = 35;
        max = 39;
    } else if (level > 30 && level <= 40) {
        min = 30;
        max = 34;
    } else if (level > 40 && level <= 47){
        min = 25;
        max = 29;
    } else {
        min = 17;
        max = 24;
    }
    std::uniform_int_distribution<int> dist(min, max);
    int numCells = dist(rd);

    std::uniform_int_distribution<int> dist2(0, 4);
    std::vector inputCell = {false, false, false, false, true};

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int index = dist2(rd);
            if (inputCell[index] && board[i][j] != 0) {
                board[i][j] = boardValues[i][j];
                numCells -= 1;
                if (numCells == 0)
                    break;
            }
        }

        if (numCells == 0)
            break;

        if (i == 8)
            i = 0;
    }
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