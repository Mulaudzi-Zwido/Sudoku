//
// Created by zwidofhelangani on 2025/09/14.
//

#ifndef SUDOKU_BUTTON_H
#define SUDOKU_BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Button {
private:
    sf::Sprite buttonIcon;
    sf::Texture texture;
    std::string filename;
    sf::Vector2f position;
    sf::Vector2f buttonScale;
public:
    Button(std::string name);
    Button(std::string name, sf::Vector2f pos);
    ~Button() = default;
    void setPosition(sf::Vector2f pos);
    void setScale(sf::Vector2f scale);
    bool selected(const sf::RenderWindow& window) const;
    sf::Sprite getSprite() const;
    void resize(unsigned int height,unsigned int width, const sf::RenderWindow& window);
};


#endif //SUDOKU_BUTTON_H