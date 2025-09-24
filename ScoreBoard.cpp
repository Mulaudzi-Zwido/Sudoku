//
// Created by zwidofhelangani on 2025/09/14.
//

#include "ScoreBoard.h"

#include <fstream>

int countLines(std::string filename) {
    int lines{0};
    std::ifstream file(filename);
    if (!file.is_open())
        return 0;
    std::string line;
    while (std::getline(file, line)) {
        lines += 1;
    }

    return lines;
}

void saveRecordsToFile(int level, sf::Time totalTime, STATUS status, std::string filename, std::vector<std::string> records) {

    std::ifstream oldRecords(filename);
    if (oldRecords.is_open()) {
        std::string record;
        while (std::getline(oldRecords, record)) {
            records.push_back(record);
        }

        oldRecords.close();
    }

    if (records.size() == 10)
        records.erase(records.begin());

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& record : records) {
        file << record << '\n';
    }

    std::string strStatus;
    switch (status) {
        case Won: strStatus = "Won";break;
        case Lost: strStatus = "Lost";break;
        case Quit: strStatus = "Quit";break;
    }
    file << "Level: " << level << " Total Time: " << totalTime.asSeconds()/60.0f << " Status: " << strStatus << '\n';
}

int getLevelFromFile(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::ofstream outfile(filename);
        outfile << 1 << '\n';
        return 1;
    }
    std::string level;
    std::getline(file, level);
    file.close();
    return std::stoi(level);
}

void saveLevelToFile(std::string filename, int level) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << level << '\n';
        file.close();
    }
}

class failedToOpenFile{};
void scoreBoard (sf::RenderWindow& window, Sudoku& board) {
    sf::RenderWindow scoreWindow(sf::VideoMode{800, 800}, "Sudoku Scoreboard");

    std::string level = "Level: " +  std::to_string(board.getLevel());
    sf::Font font;
    try {
        if (!font.loadFromFile("Fonts/Calm Serif (DemoVersion).otf"))
            throw failedToOpenFile();
    } catch (failedToOpenFile) {
        std::cerr << "Failed To Open File.\n";
        std::exit(EXIT_FAILURE);
    }
    sf::Text levelTxt;
    levelTxt.setFont(font);
    levelTxt.setString(level);
    levelTxt.setPosition(scoreWindow.getSize().x/30.0f, 0.0f);
    levelTxt.setFillColor(sf::Color::Black);

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
        scoreWindow.draw(levelTxt);
        scoreWindow.display();
    }
}