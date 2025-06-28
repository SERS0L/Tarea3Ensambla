#include "../include/Theme.hpp"

Theme createDarkTheme() {
    return Theme{
        sf::Color(32, 33, 35),
        sf::Color(224, 224, 224),
        sf::Color(60, 60, 60),
        sf::Color(100, 100, 100),
        sf::Color(80, 80, 80),
        sf::Color(40, 40, 40)
    };
}

Theme createLightTheme() {
    return Theme{
        sf::Color(220, 220, 220),
        sf::Color(50, 50, 50),
        sf::Color(200, 200, 200),
        sf::Color(150, 150, 150),
        sf::Color(180, 180, 180),
        sf::Color(160, 160, 160)
    };
}