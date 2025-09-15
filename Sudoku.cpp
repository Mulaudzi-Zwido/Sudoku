//
// Created by zwidofhelangani on 2025/09/14.
//

#include "Sudoku.h"
#include <random>
#include <algorithm>
#include <cstdlib>

Sudoku::Sudoku() {
    board = std::vector<std::vector<int>>(9, std::vector<int>(9, 0));
    boardValues = std::vector<std::vector<int>>(9, std::vector<int>(9, 0));
    try {
        if (!font.loadFromFile("Fonts/Calm Serif (DemoVersion).otf"))
            throw FailedToLoadFont();
    }catch (FailedToLoadFont) {
        std::cerr << "Failed To Open File.\n";
        std::exit(EXIT_FAILURE);
    }
}

void Sudoku::generateWindowBoard(sf::RenderWindow& parentWindow) {

    sf::Vector2f size = static_cast<sf::Vector2f>(parentWindow.getSize());
    size.x *= 0.9f;
    size.y *= 0.9f;

    sf::Vector2f blockSize;
    blockSize.x = size.x/9;
    blockSize.y = size.y/9;

    sf::Vector2f blockPos;
    blockPos.x = size.x/9;
    blockPos.y = size.y/9;

    blocks = std::vector<std::vector<sf::RectangleShape>>(9, std::vector<sf::RectangleShape>(9, sf::RectangleShape(blockSize)));

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            blocks[i][j].setPosition(sf::Vector2f{blockPos.x * (i + 0.5f), blockPos.y * (j + 0.95f)});
        }
    }

    for (auto& blockV : blocks) {
        for (auto& block : blockV) {
            block.setFillColor(sf::Color{237, 237, 237, 255});
            block.setOutlineColor(sf::Color::Black);
            block.setOutlineThickness(1.f);
        }
    }
}

bool Sudoku::valid(int row, int col, int value) const {
    for (int i = 0; i < 9; ++i)
        if (boardValues[i][col] == value)
            return false;

    for (int i = 0; i < 9; ++i)
        if (boardValues[row][i] == value)
            return false;

    int startRow = row - row%3;
    int startCol = col - col%3;

    for (int i = startRow; i < startRow + 3; ++i) {
        for (int j = startCol; j < startCol + 3; ++j)
            if (boardValues[i][j] == value)
                return false;
    }

    return true;
}

int cellsToRemove(int level) {
    std::random_device rd;
    int min, max;

    if (level <= 10) {
        min = 45;
        max = 49;
    } else if (level <= 20) {
        min = 40;
        max = 44;
    } else if (level <= 30) {
        min = 35;
        max = 39;
    } else if (level <= 40) {
        min = 30;
        max = 34;
    } else if (level <= 47){
        min = 25;
        max = 29;
    } else {
        min = 17;
        max = 24;
    }

    std::uniform_int_distribution<int> dist(min, max);
    int numCells = dist(rd);

    return numCells;
}

void Sudoku::generateBoard() {
    if (fillGrid()) {
        board = boardValues; // Save full solution

        int cellsRemoved = cellsToRemove(level); // Number of cells to remove

        // Create a list of all cell positions
        std::vector<std::pair<int, int>> positions;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                positions.emplace_back(i, j);
            }
        }

        // Shuffle positions
        std::shuffle(positions.begin(), positions.end(), std::mt19937{std::random_device{}()});

        // Remove the required number of cells
        for (int i = 0; i < cellsRemoved; ++i) {
            auto [row, col] = positions[i];
            board[row][col] = 0;
        }

    } else {
        std::cerr << "Failed to generate a valid Sudoku board." << std::endl;
    }
}

void findEmptyCell(const std::vector<std::vector<int>>& board, int& row, int& col) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] == 0) {
                row = i;
                col = j;
                return;
            }
        }
    }

    row = -1;
    col = -1;
}

bool Sudoku::fillGrid() {
    int row, col;

    findEmptyCell(boardValues, row, col);

    if (col == -1 && row == -1)
        return true;

    std::vector<int> numbers = {1,2,3,4,5,6,7,8,9};

    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(numbers), std::end(numbers), rng);

    for (int num : numbers) {
        if (valid(row, col, num)) {
            boardValues[row][col] = num;
            if (fillGrid())//Will return true if there is no empty cell left
                return true;
            boardValues[row][col] = 0;
        }
    }

    return false;
}

void Sudoku::printBoard(sf::RenderWindow &mainW) {
    sf::RenderWindow window(sf::VideoMode{800, 800}, "Sudoku");

    window.setFramerateLimit(60);
    fillGrid();
    generateBoard();
    generateWindowBoard(window);

    line.setFont(font);
    line.setString("Level: " + std::to_string(level));
    line.setPosition(window.getSize().x/8.0f, 0.0f);
    line.setCharacterSize(50);
    line.setFillColor(sf::Color::Black);
    while (window.isOpen()) {
        sf::Event gameEvent;
        while (window.pollEvent(gameEvent)) {
            if (gameEvent.type == sf::Event::Closed) {
                mainW.setVisible(true);
                window.close();
            }
        }
        window.clear(sf::Color::White);
        for (auto& blockV : blocks) {
            for (auto& block : blockV) {
                window.draw(block);
            }
        }
        window.draw(line);
        window.display();
    }
}
