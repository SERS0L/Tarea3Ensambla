#include "../include/LanguageScreen.hpp"
#include "../include/Utils.hpp"
#include <iostream>

LanguageScreen::LanguageScreen(sf::RenderWindow& win) : window(win) {
    darkTheme = createDarkTheme();
    lightTheme = createLightTheme();
    currentTheme = darkTheme;

    if (!font.loadFromFile("resources/fonts/Ubuntu-Regular.ttf"))  {
        std::cerr << "Error al cargar la fuente" << std::endl;
        exit(1);
    }

    if (!themeIconTexture.loadFromFile("resources/images/theme_icon.png") ||
        !flagTextureCR.loadFromFile("resources/images/costa_rica_flag.png") ||
        !flagTextureUSA.loadFromFile("resources/images/usa_flag.png") ||
        !flagTextureBrasil.loadFromFile("resources/images/brasil_flag.png") ||
        !nextButtonTexture.loadFromFile("resources/images/next_button.png")) {
        std::cerr << "Error al cargar las imagenes" << std::endl;
        exit(1);
    }

    themeIcon.setTexture(themeIconTexture);
    nextButtonSprite.setTexture(nextButtonTexture);

    themeButton.setSize({40, 40});
    themeButton.setPosition(25, 25);

    float scale = 80.0f / std::max(themeIcon.getLocalBounds().width, themeIcon.getLocalBounds().height);
    themeIcon.setScale(scale, scale);
    themeIcon.setPosition(
        themeButton.getPosition().x + (themeButton.getSize().x - themeIcon.getGlobalBounds().width)/2,
        themeButton.getPosition().y + (themeButton.getSize().y - themeIcon.getGlobalBounds().height)/2
    );

    title.setFont(font);
    title.setString("Select a language");
    title.setCharacterSize(30);
    title.setPosition(430, 60);

    selectionText.setFont(font);
    selectionText.setCharacterSize(24);
    selectionText.setStyle(sf::Text::Bold);

    nextButton.setSize({50, 50});
    nextButton.setPosition(520, 500);
    nextButtonSprite.setTexture(nextButtonTexture);


    buttonCR.setSize({200, 120});
    buttonCR.setPosition(150, 200);
    flagSpriteCR.setTexture(flagTextureCR);

    buttonUSA.setSize({200, 120});
    buttonUSA.setPosition(450, 200);
    flagSpriteUSA.setTexture(flagTextureUSA);

    buttonBrasil.setSize({200, 120});
    buttonBrasil.setPosition(750, 200);
    flagSpriteBrasil.setTexture(flagTextureBrasil);

    scaleAndCenter(flagSpriteCR, buttonCR);
    scaleAndCenter(flagSpriteUSA, buttonUSA);
    scaleAndCenter(flagSpriteBrasil, buttonBrasil);
    scaleAndCenter(nextButtonSprite, nextButton);

    

    applyTheme();
}

void LanguageScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
        mouseOnThemeButton = themeButton.getGlobalBounds().contains(mousePos);
        mouseOnNextButton = nextButton.getGlobalBounds().contains(mousePos) && languageSelected;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);

        if (themeButton.getGlobalBounds().contains(pos)) {
            isDarkTheme = !isDarkTheme;
            currentTheme = isDarkTheme ? darkTheme : lightTheme;
            applyTheme();
        } else if (buttonCR.getGlobalBounds().contains(pos)) {
            selectionText.setString("Idioma seleccionado: Espanol");
            selectionText.setPosition(390, 350);
            selectedLanguage = "Spanish";
            languageSelected = true;
        } else if (buttonUSA.getGlobalBounds().contains(pos)) {
            selectionText.setString("Selected language: English");
            selectionText.setPosition(400, 350);
            selectedLanguage = "English";
            languageSelected = true;
        } else if (buttonBrasil.getGlobalBounds().contains(pos)) {
            selectionText.setString("Idioma selecionado: Portugues");
            selectionText.setPosition(385, 350);
            selectedLanguage = "Portuguese";
            languageSelected = true;
        } else if (nextButton.getGlobalBounds().contains(pos) && languageSelected) {
            goToNextScreen = true;
        }
    }
}

void LanguageScreen::update() {
    themeButton.setFillColor(mouseOnThemeButton ? currentTheme.hoverColor : currentTheme.buttonColor);
    if (languageSelected) {
        nextButton.setFillColor(mouseOnNextButton ? currentTheme.hoverColor : currentTheme.buttonColor);
        nextButtonSprite.setColor(mouseOnNextButton ? currentTheme.textColor : sf::Color::White);
    } else {
        nextButton.setFillColor(currentTheme.disabledColor);
        nextButtonSprite.setColor(currentTheme.disabledColor);
    }
}

void LanguageScreen::draw() {
    window.clear(currentTheme.backgroundColor);
    window.draw(title);
    window.draw(buttonCR);
    window.draw(buttonUSA);
    window.draw(buttonBrasil);
    window.draw(flagSpriteCR);
    window.draw(flagSpriteUSA);
    window.draw(flagSpriteBrasil);
    window.draw(selectionText);
    window.draw(nextButton);
    window.draw(nextButtonSprite);
    window.draw(themeButton);
    window.draw(themeIcon);
}

void LanguageScreen::applyTheme() {
    title.setFillColor(currentTheme.textColor);
    selectionText.setFillColor(currentTheme.textColor);

    buttonCR.setFillColor(currentTheme.buttonColor);
    buttonCR.setOutlineColor(currentTheme.outlineColor);

    buttonUSA.setFillColor(currentTheme.buttonColor);
    buttonUSA.setOutlineColor(currentTheme.outlineColor);

    buttonBrasil.setFillColor(currentTheme.buttonColor);
    buttonBrasil.setOutlineColor(currentTheme.outlineColor);

    themeButton.setOutlineThickness(1);
    themeButton.setOutlineColor(currentTheme.outlineColor);

    nextButton.setOutlineThickness(2);
    nextButton.setOutlineColor(currentTheme.outlineColor);
}

bool LanguageScreen::shouldSwitchScreen() const {
    return goToNextScreen;
}

std::string LanguageScreen::getSelectedLanguage() const {
    return selectedLanguage;
}
