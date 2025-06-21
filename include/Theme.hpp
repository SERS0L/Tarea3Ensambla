#ifndef THEME_HPP
#define THEME_HPP

#include <SFML/Graphics.hpp>

struct Theme {
    sf::Color backgroundColor;
    sf::Color textColor;
    sf::Color buttonColor;
    sf::Color outlineColor;
    sf::Color hoverColor;
    sf::Color disabledColor;
};

Theme createDarkTheme();
Theme createLightTheme();

#endif