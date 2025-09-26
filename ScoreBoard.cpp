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
void saveRecordsToFile(int level, sf::Time totalTime, STATUS status, std::string filename) {
    std::vector<std::string> records;
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
        case Completed: strStatus = "Completed";break;
        case Lost: strStatus = "Lost";break;
        case Quit: strStatus = "Quit";break;
    }
    file << "Level: " << level << " Total Time: " << totalTime.asSeconds()/60.0f << " Status: " << strStatus << '\n';
}
void readRecordsFromFile(std::vector<std::string> &records, std::vector<bool>& status, std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        return;
    std::string line;
    while (std::getline(file, line)) {
        std::string statusStr = line.substr(line.size() - 4);
        std::cout << statusStr << std::endl;
        if (statusStr == "Lost" || statusStr == "Quit")
            status.push_back(false);
        else
            status.push_back(true);
        records.push_back(line);
    }
}
class failedToOpenFile{};
void scoreBoard (sf::RenderWindow& window, Sudoku& board) {
    sf::RenderWindow scoreWindow(sf::VideoMode{800, 800}, "Sudoku Scoreboard");

    std::string level = "Current Level: " +  std::to_string(board.getLevel());
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
    levelTxt.setPosition(scoreWindow.getPosition().x/2.0f, 0.0f);
    levelTxt.setFillColor(sf::Color::Black);

    std::string topOfRecords = "Last 10 Records: ";
    sf::Text recordText;
    recordText.setFont(font);
    recordText.setFillColor(sf::Color::Black);
    recordText.setString(topOfRecords);
    recordText.setPosition(scoreWindow.getSize().x/30.0f, scoreWindow.getPosition().y/5.0f);

    //Get Records
    std::vector<std::string> records;
    std::vector<bool> status;
    readRecordsFromFile(records, status, "GameFiles/records.txt");
    std::vector<sf::Text> recordLines;
    const float startX = 50.0f;
    const float startY = 50.0f;
    const float lineSpacing = 30;

    for (int i = 0; i < static_cast<int>(records.size()); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(25);
        text.setString(records[i]);

        text.setFillColor(status[i] ? sf::Color::Green : sf::Color::Red);
        text.setPosition(startX, startY + (i + 0.5f) * lineSpacing);
        recordLines.push_back(text);
    }

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
        for (auto& record : recordLines)
            scoreWindow.draw(record);
        scoreWindow.draw(recordText);
        scoreWindow.display();
    }
}