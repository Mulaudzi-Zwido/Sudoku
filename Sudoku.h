//
// Created by zwidofhelangani on 2025/09/14.
//

#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H

#include "Button.h"
#include <vector>
#include <string>

enum STATUS {
    Won = 0,
    Lost = 1,
    Quit = 2
};

class Sudoku {
private:
    int level {1};
    int errors{0};
    sf::Time totalTime;
    STATUS status;

    sf::Font font;
    std::vector<sf::Text> texts;
    sf::Text line;
    std::vector<std::vector<int>> board;
    std::vector<std::vector<int>> boardValues;
    std::vector<std::vector<sf::RectangleShape>>blocks;
    std::vector<sf::VertexArray> boxLines;
    std::vector<std::string> records;

    void generateBoard(); // Fills boardValues with the complete sudoku board values
    bool fillGrid();
    bool valid(int row, int col, int value) const;
    void generateWindowBoard(sf::RenderWindow& parentWindow);
    void createBoxOutline();

    bool correctNum(int row, int col, int val);
    bool userInput(const sf::RenderWindow& window);
    void resetBoard();

    class FailedToLoadFont{};
public:
    Sudoku();
    ~Sudoku() = default;
    void printBoard(sf::RenderWindow& mainW); //Prints the sudoku board as an interactive window
    void setLevel(int lv);
    int getLevel() const;
    sf::Time getTime() const;
    STATUS getStatus() const;
    const std::vector<std::string>& getRecords();
};

#endif //SUDOKU_SUDOKU_H
