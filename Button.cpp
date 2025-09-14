//
// Created by zwidofhelangani on 2025/09/14.
//

#include "Button.h"

#include <utility>

Button::Button(std::string name) {
    filename = std::move(name);

    texture.loadFromFile(filename);

    buttonIcon.setTexture(texture);

    buttonScale = {1.0f, 1.0f};
}

Button::Button(std::string name, const sf::Vector2f pos) {
    filename = std::move(name);
    texture.loadFromFile(filename);

    buttonIcon.setTexture(texture);

    buttonScale = {1.0f, 1.0f};

    position = pos;
    buttonIcon.setPosition(position);
}

void Button::setScale(const sf::Vector2f scale) {
    buttonScale = scale;
    buttonIcon.setScale(buttonScale);
}

void Button::setPosition(const sf::Vector2f pos) {
    position = pos;
    buttonIcon.setPosition(position);
}

bool Button::selected(const sf::RenderWindow& window) const {
    sf::Vector2i cursor = sf::Mouse::getPosition(window);
    sf::Vector2f worldCursor = window.mapPixelToCoords(cursor);

    sf::FloatRect bound = buttonIcon.getGlobalBounds();

    bool cursorInButton = bound.contains(worldCursor);
    if (!cursorInButton)
        return false;

    bool keyEntered = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) || sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (!keyEntered)
        return false;

    return true;
}

sf::Sprite Button::getSprite() const {
    return buttonIcon;
}

void Button::resize(unsigned int height,unsigned int width, const sf::RenderWindow& window) {
    //Scales the buttons appropriately
    float scaleX = static_cast<float>(window.getSize().x / width);
    float scaleY = static_cast<float>(window.getSize().y / height);

    this->buttonScale.x *= scaleX;
    this->buttonScale.y *= scaleY;

    //Puts the buttons in the correct place
    this->position.x = static_cast<float>(window.getSize().x - this->buttonIcon.getGlobalBounds().width/2.0f);
    this->position.y = static_cast<float>(window.getSize().y - this->buttonIcon.getGlobalBounds().height/2.0f);
}

