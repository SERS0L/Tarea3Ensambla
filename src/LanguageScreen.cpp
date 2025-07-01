#include "../include/LanguageScreen.hpp"
#include "../include/Utils.hpp"
#include "../include/ButtonsLogic.hpp"
#include <iostream>

LanguageScreen::LanguageScreen(sf::RenderWindow& win) : window(win) {
    darkTheme = {
        uintToColor(getDarkThemeBackgroundColor()),
        uintToColor(getDarkThemeTextColor()),
        uintToColor(getDarkThemeButtonColor()),
        uintToColor(getDarkThemeOutlineColor()),
        uintToColor(getDarkThemeHoverColor()),
        uintToColor(getDarkThemeDisabledColor())
    };
    
    lightTheme = {
        uintToColor(getLightThemeBackgroundColor()),
        uintToColor(getLightThemeTextColor()),
        uintToColor(getLightThemeButtonColor()),
        uintToColor(getLightThemeOutlineColor()),
        uintToColor(getLightThemeHoverColor()),
        uintToColor(getLightThemeDisabledColor())
    };
    
    currentTheme = darkTheme;

    if (!font.loadFromFile("resources/fonts/Ubuntu-Regular.ttf")) {
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
    title.setCharacterSize(50);
    title.setPosition(610, 60);

    selectionText.setFont(font);
    selectionText.setCharacterSize(30);
    selectionText.setStyle(sf::Text::Bold);

    nextButton.setSize({80, 80});
    nextButton.setPosition(770, 750);
    nextButtonSprite.setTexture(nextButtonTexture);


    buttonCR.setSize({290, 180});
    buttonCR.setPosition(182, 300);
    flagSpriteCR.setTexture(flagTextureCR);

    buttonUSA.setSize({290, 180});
    buttonUSA.setPosition(654,300);
    flagSpriteUSA.setTexture(flagTextureUSA);

    buttonBrasil.setSize({290, 180});
    buttonBrasil.setPosition(1126, 300);
    flagSpriteBrasil.setTexture(flagTextureBrasil);


    increaseFontButton.setSize({40, 40});
    increaseFontButton.setPosition(1542, 20);
    increaseFontButton.setFillColor(currentTheme.buttonColor);
    increaseFontButton.setOutlineThickness(1);
    increaseFontButton.setOutlineColor(currentTheme.outlineColor);
    
    plusText.setFont(font);
    plusText.setString("+");
    plusText.setCharacterSize(30);
    plusText.setFillColor(currentTheme.textColor);
    plusText.setPosition(
        increaseFontButton.getPosition().x + 10,
        increaseFontButton.getPosition().y + 3
    );
    
    // Botón "-"
    decreaseFontButton.setSize({40, 40});
    decreaseFontButton.setPosition(1498, 20);
    decreaseFontButton.setFillColor(currentTheme.buttonColor);
    decreaseFontButton.setOutlineThickness(1);
    decreaseFontButton.setOutlineColor(currentTheme.outlineColor);
    
    minusText.setFont(font);
    minusText.setString("-");
    minusText.setCharacterSize(30);
    minusText.setFillColor(currentTheme.textColor);
    minusText.setPosition(
        decreaseFontButton.getPosition().x + 10,
        decreaseFontButton.getPosition().y + 3
    );



    boldToggleButton.setSize({40, 40});
    boldToggleButton.setPosition(1450, 20);
    boldToggleButton.setFillColor(currentTheme.buttonColor);
    boldToggleButton.setOutlineThickness(1);
    boldToggleButton.setOutlineColor(currentTheme.outlineColor);
    
    boldToggleText.setFont(font);
    boldToggleText.setString("B");
    boldToggleText.setCharacterSize(30);
    boldToggleText.setStyle(sf::Text::Bold);
    boldToggleText.setFillColor(currentTheme.textColor);
    boldToggleText.setPosition(
    boldToggleButton.getPosition().x + 22,
    boldToggleButton.getPosition().y + 6
);

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
            // Usamos la función ASM para cambiar el tema
            isDarkTheme = changeTheme(isDarkTheme);
            currentTheme = isDarkTheme ? darkTheme : lightTheme;
            applyTheme();
        } else if (buttonCR.getGlobalBounds().contains(pos)) {
            selectionText.setString("Idioma seleccionado: Espanol");
            selectionText.setPosition(590, 500);
            selectedLanguage = "Spanish";
            languageSelected = true;
        } else if (buttonUSA.getGlobalBounds().contains(pos)) {
            selectionText.setString("Selected language: English");
            selectionText.setPosition(600, 500);
            selectedLanguage = "English";
            languageSelected = true;
        } else if (buttonBrasil.getGlobalBounds().contains(pos)) {
            selectionText.setString("Idioma selecionado: Portugues");
            selectionText.setPosition(580, 500);
            selectedLanguage = "Portuguese";
            languageSelected = true;
        } else if (nextButton.getGlobalBounds().contains(pos) && languageSelected) {
            goToNextScreen = true;
        } else if (increaseFontButton.getGlobalBounds().contains(pos)) {
            // Usamos la función ASM para aumentar el tamaño de fuente
            unsigned int titleSize = increaseFontSize(title.getCharacterSize());
            unsigned int selectionSize = increaseFontSize(selectionText.getCharacterSize());
            
            if (titleSize != title.getCharacterSize()) {
                title.setCharacterSize(titleSize);
                title.setPosition((window.getSize().x - title.getLocalBounds().width) / 2.f, 60);
            }
            
            if (selectionSize != selectionText.getCharacterSize()) {
                selectionText.setCharacterSize(selectionSize);
                selectionText.setPosition((window.getSize().x - selectionText.getLocalBounds().width) / 2.f, 350);
            }
            selectedFontSize = selectionText.getCharacterSize();
        }
        else if (decreaseFontButton.getGlobalBounds().contains(pos)) {
            // Usamos la función ASM para disminuir el tamaño de fuente
            unsigned int titleSize = decreaseFontSize(title.getCharacterSize());
            unsigned int selectionSize = decreaseFontSize(selectionText.getCharacterSize());
            
            if (titleSize != title.getCharacterSize()) {
                title.setCharacterSize(titleSize);
                title.setPosition((window.getSize().x - title.getLocalBounds().width) / 2.f, 60);
            }
            
            if (selectionSize != selectionText.getCharacterSize()) {
                selectionText.setCharacterSize(selectionSize);
                selectionText.setPosition((window.getSize().x - selectionText.getLocalBounds().width) / 2.f, 350);
            }
            selectedFontSize = selectionText.getCharacterSize();
        }
        else if (boldToggleButton.getGlobalBounds().contains(pos)) {
            isBold = !isBold;
            title.setStyle(isBold ? sf::Text::Bold : sf::Text::Regular);
            selectionText.setStyle(isBold ? sf::Text::Bold : sf::Text::Regular);
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

bool LanguageScreen::shouldSwitchScreen() const {
    return goToNextScreen;
}

std::string LanguageScreen::getSelectedLanguage() const {
    return selectedLanguage;
}

unsigned int LanguageScreen::getSelectedFontSize() const {
    return selectedFontSize;
}

Theme LanguageScreen::getCurrentTheme() const {
    return currentTheme;
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

    increaseFontButton.setFillColor(currentTheme.buttonColor);
    increaseFontButton.setOutlineThickness(1);
    increaseFontButton.setOutlineColor(currentTheme.outlineColor);
    plusText.setFillColor(currentTheme.textColor);
    
    decreaseFontButton.setFillColor(currentTheme.buttonColor);
    decreaseFontButton.setOutlineThickness(1);
    decreaseFontButton.setOutlineColor(currentTheme.outlineColor);
    minusText.setFillColor(currentTheme.textColor);

    boldToggleButton.setFillColor(currentTheme.buttonColor);
    boldToggleButton.setOutlineColor(currentTheme.outlineColor);
    boldToggleText.setFillColor(currentTheme.textColor);
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
    window.draw(increaseFontButton);
    window.draw(plusText);
    window.draw(decreaseFontButton);
    window.draw(minusText);
    window.draw(boldToggleButton);
    window.draw(boldToggleText);
}





