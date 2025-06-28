#ifndef UTILS_HPP
#define UTILS_HPP

#include <SFML/Graphics.hpp>
#include <algorithm>

inline void scaleAndCenter(sf::Sprite& sprite, const sf::RectangleShape& container) {
    float scale = std::min(
        container.getSize().x / sprite.getLocalBounds().width,
        container.getSize().y / sprite.getLocalBounds().height
    );
    sprite.setScale(scale, scale);
    sprite.setPosition(
        container.getPosition().x + (container.getSize().x - sprite.getGlobalBounds().width) / 2,
        container.getPosition().y + (container.getSize().y - sprite.getGlobalBounds().height) / 2
    );
}

#endif