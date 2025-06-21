#ifndef LANGUAGE_SCREEN_HPP
#define LANGUAGE_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "Theme.hpp"

class LanguageScreen {
public:
    LanguageScreen(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);
    void update();
    void draw();

private:
    void applyTheme();

    sf::RenderWindow& window;
    sf::Font font;

    Theme darkTheme, lightTheme, currentTheme;
    bool isDarkTheme = true;
    bool languageSelected = false;
    bool mouseOnNextButton = false;
    bool mouseOnThemeButton = false;
    std::string selectedLanguage;

    sf::Text title, selectionText;
    sf::RectangleShape buttonCR, buttonUSA, buttonBrasil, nextButton, themeButton;
    sf::Texture flagTextureCR, flagTextureUSA, flagTextureBrasil, themeIconTexture, nextButtonTexture;
    sf::Sprite flagSpriteCR, flagSpriteUSA, flagSpriteBrasil, themeIcon, nextButtonSprite;
};

#endif
