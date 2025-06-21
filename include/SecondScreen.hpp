#ifndef SECOND_SCREEN_HPP
#define SECOND_SCREEN_HPP

#include <SFML/Graphics.hpp>

class SecondScreen {
public:
    SecondScreen(sf::RenderWindow& win, const std::string& language);
    void handleEvent(const sf::Event& event);
    void update();
    void draw();

private:
    sf::RenderWindow& window;
    sf::Font font;

    sf::Text label;
    sf::RectangleShape micButton;
    sf::Texture micTexture;
    sf::Sprite micSprite;

    sf::RectangleShape textBox;
    sf::Text inputText;
    std::string currentInput;
    std::string selectedLanguage;
    bool isFocused = false;
};

#endif
