//
// Created by zwidofhelangani on 2025/09/14.
//

#ifndef SUDOKU_SCOREBOARD_H
#define SUDOKU_SCOREBOARD_H

#include "Button.h"
#include "Sudoku.h"

void scoreBoard(sf::RenderWindow& window, Sudoku& board);
int countLines(std::string filename);
void saveRecordsToFile(int level, sf::Time totalTime, STATUS status, std::string filename);
void readRecordsFromFile(std::vector<std::string> &records, std::vector<bool>& status, std::string filename);
void saveLevelToFile(std::string filename, int level);
int getLevelFromFile(std::string filename);


#endif //SUDOKU_SCOREBOARD_H