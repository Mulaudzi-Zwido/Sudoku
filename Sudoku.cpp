//
// Created by zwidofhelangani on 2025/09/14.
//

#include "Sudoku.h"
#include <random>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <bits/this_thread_sleep.h>
#include <SFML/System.hpp>

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

void Sudoku::resetBoard() {
    board = std::vector<std::vector<int>>(9, std::vector<int>(9, 0));
    boardValues = std::vector<std::vector<int>>(9, std::vector<int>(9, 0));
    errors = 0;
}

void makeBox(sf::VertexArray& box, int row, int col, const std::vector<std::vector<sf::RectangleShape>>& blocks) {
    box[0].position = blocks[row][col].getPosition();
    box[4].position = blocks[row][col].getPosition();

    box[1].position.y = blocks[row][col].getPosition().y;
    box[3].position.x = blocks[row][col].getPosition().x;

    int lastRow = row + 3;
    int lastCol = col + 3;
    int blockXSize =  blocks[0][0].getSize().x;
    int blockYSize =  blocks[0][0].getSize().y;
    if (lastRow == 9) {
        lastRow -= 1;
        box[2].position.y = blocks[lastRow][col].getPosition().y + static_cast<float>(blockYSize);
        box[3].position.y = blocks[lastRow][col].getPosition().y + static_cast<float>(blockYSize);
    }else {
        box[2].position.y = blocks[lastRow][col].getPosition().y;
        box[3].position.y = blocks[lastRow][col].getPosition().y;
    }

    if (lastCol== 9) {
        lastCol -= 1;
        box[1].position.x = blocks[row][lastCol].getPosition().x + static_cast<float>(blockXSize);
        box[2].position.x = blocks[row][lastCol].getPosition().x + static_cast<float>(blockXSize);
    } else {
        box[1].position.x = blocks[row][lastCol].getPosition().x;
        box[2].position.x = blocks[row][lastCol].getPosition().x;
    }
}

void Sudoku::createBoxOutline() {
    //Box 1
    {
        sf::VertexArray box(sf::LineStrip, 5);
        makeBox(box, 0,0, blocks);
        boxLines.push_back(box);
    }
    //Box 2
    {
         sf::VertexArray box(sf::LineStrip, 5);
         makeBox(box,0,3, blocks);
         boxLines.push_back(box);
    }
    //Box3
    {
         sf::VertexArray box(sf::LineStrip, 5);
         makeBox(box,0,6, blocks);
         boxLines.push_back(box);
    }
    //Box4
    {
         sf::VertexArray box(sf::LineStrip, 5);
         makeBox(box,3,0, blocks);
         boxLines.push_back(box);
    }
    //Box5
    {
         sf::VertexArray box(sf::LineStrip, 5);
         makeBox(box,3,3, blocks);
         boxLines.push_back(box);
    }
    //Box6
    {
         sf::VertexArray box(sf::LineStrip, 5);
         makeBox(box,3,6, blocks);
         boxLines.push_back(box);
    }
    //Box7
    {
         sf::VertexArray box(sf::LineStrip, 5);
         makeBox(box, 6,0, blocks);
         boxLines.push_back(box);
    }
    //Box8
    {
         sf::VertexArray box(sf::LineStrip, 5);
         makeBox(box, 6,3, blocks);
         boxLines.push_back(box);
    }
    //Box9
    {
         sf::VertexArray box(sf::LineStrip, 5);
         makeBox(box, 6,6, blocks);
         boxLines.push_back(box);
    }

    for (auto& box : boxLines) {
        box[0].color = sf::Color::Black;
        box[1].color = sf::Color::Black;
        box[2].color = sf::Color::Black;
        box[3].color = sf::Color::Black;
        box[4].color = sf::Color::Black;
    }
}

void Sudoku::generateWindowBoard(sf::RenderWindow& parentWindow) {
    sf::Vector2f size = static_cast<sf::Vector2f>(parentWindow.getSize());
    size.x *= 0.9f;
    size.y *= 0.9f;

    sf::Vector2f blockSize;
    blockSize.x = size.x / 9;
    blockSize.y = size.y / 9;

    blocks = std::vector(9, std::vector<sf::RectangleShape>(9, sf::RectangleShape(blockSize)));

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            blocks[row][col].setPosition(sf::Vector2f{
                blockSize.x * (col + 0.5f),
                blockSize.y * (row + 0.7f)
            });
            blocks[row][col].setFillColor(sf::Color::White);
            blocks[row][col].setOutlineColor(sf::Color{237,237,237,255});
            blocks[row][col].setOutlineThickness(1.f);
        }
    }

    createBoxOutline();

    // Clear previous texts
    texts.clear();

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            sf::Text text;
            text.setFont(font);
            text.setFillColor(sf::Color::Black);
            text.setCharacterSize(50);

            if (board[row][col] != 0)
                text.setString(std::to_string(board[row][col]));
            else
                text.setString("");

            // Center the text in the block
            sf::Vector2f pos = blocks[row][col].getPosition();
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
            text.setPosition(pos.x + blockSize.x / 2.f, pos.y + blockSize.y / 2.f);

            texts.push_back(text);
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

STATUS Sudoku::getStatus() const {
    return status;
}

int Sudoku::getLevel() const {
    return level;
}

const std::vector<std::string> &Sudoku::getRecords() {
    return records;
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

void Sudoku::setLevel(int lv) {
    level = lv;
}

bool Sudoku::correctNum(int row, int col, int val) {
    if (boardValues[row][col] == val)
        return true;
    errors += 1;
    return false;
}

bool Sudoku::userInput(const sf::RenderWindow& window) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            sf::Mouse cursor;
            sf::FloatRect block = blocks[i][j].getGlobalBounds();
            sf::Vector2i cursorPos = cursor.getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(cursorPos);

            if (block.contains(worldPos)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
                    if (correctNum(i,j, 1)) {
                        board[i][j] = 1;
                        return true;
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                    if (correctNum(i,j, 2)) {
                        board[i][j] = 2;
                        return true;
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
                    if (correctNum(i,j, 3)) {
                        board[i][j] = 3;
                        return true;
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
                    if (correctNum(i,j, 4)) {
                        board[i][j] = 4;
                        return true;
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
                    if (correctNum(i,j, 5)) {
                        board[i][j] = 5;
                        return true;
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
                    if (correctNum(i,j, 6)) {
                        board[i][j] = 6;
                        return true;
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
                    if (correctNum(i,j, 7)) {
                        board[i][j] = 7;
                        return true;
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
                    if (correctNum(i,j, 8)) {
                        board[i][j] = 8;
                        return true;
                    }
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)) {
                    if (correctNum(i,j, 9)) {
                        board[i][j] = 9;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

sf::Time Sudoku::getTime() const {
    return totalTime;
}

void Sudoku::printBoard(sf::RenderWindow &mainW) {
    sf::RenderWindow window(sf::VideoMode{800, 800}, "Sudoku");

    window.setFramerateLimit(60);
    fillGrid();
    generateBoard();
    generateWindowBoard(window);

    std::cout << "Boxes: " << boxLines.size() << "\n";

    line.setFont(font);
    line.setString("Level: " + std::to_string(level));
    line.setPosition(window.getSize().x/8.0f, 0.0f);
    line.setCharacterSize(40);
    line.setFillColor(sf::Color::Black);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event gameEvent;
        while (window.pollEvent(gameEvent)) {
            if (gameEvent.type == sf::Event::Closed) {
                totalTime = clock.getElapsedTime();
                status = Quit;
                mainW.setVisible(true);
                window.close();
            } else if (gameEvent.type == sf::Event::KeyPressed) {
                bool updated = userInput(window);
                if (updated) {
                    generateWindowBoard(window);
                }
            }
        }

        window.clear(sf::Color::White);

        for (auto& blockV : blocks) {
            for (auto& block : blockV) {
                window.draw(block);
            }
        }

        if (errors == 3) {
            sf::Texture gameOverTexture;
            gameOverTexture.loadFromFile("Textures/gameOver.png");
            sf::Sprite gameOverImage;
            gameOverImage.setTexture(gameOverTexture);
            gameOverImage.setScale(0.15f, 0.15f);
            gameOverImage.setPosition(window.getSize().x/6.0f, window.getSize().y/2.85f);
            window.draw(gameOverImage);

            for (auto text : texts) {
                if (text.getString() != "0")
                    window.draw(text);
            }

            window.draw(line);

            window.display();
            resetBoard();
            auto delay = std::chrono::seconds(2);
            std::this_thread::sleep_for(delay);
            totalTime = clock.getElapsedTime();
            status = Lost;
            mainW.setVisible(true);
            window.close();
        }

        if (board == boardValues) {
            sf::Texture winTexture;
            winTexture.loadFromFile("Textures/win.png");
            sf::Sprite winImage;
            winImage.setTexture(winTexture);
            winImage.setScale(0.15f, 0.15f);
            winImage.setPosition(window.getSize().x/6.0f, window.getSize().y/2.85f);
            window.draw(winImage);

            for (auto text : texts) {
                if (text.getString() != "0")
                    window.draw(text);
            }

            window.draw(line);

            window.display();
            resetBoard();
            auto delay = std::chrono::seconds(2);
            std::this_thread::sleep_for(delay);
            totalTime = clock.getElapsedTime();
            status = Won;
            mainW.setVisible(true);
            window.close();
        }

        for (auto text : texts) {
            if (text.getString() != "0")
                window.draw(text);
        }

        window.draw(line);
        for (const auto& box : boxLines) {
            window.draw(box);
        }
        window.display();
    }
}