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

// Declaración de la función de conversión
sf::Color uintToColor(unsigned int value);

// Declaraciones de las funciones para crear temas
Theme createDarkTheme();
Theme createLightTheme();

#endif