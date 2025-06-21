#include <SFML/Graphics.hpp>
#include <iostream>

// Estructura para manejar los temas
struct Theme {
    sf::Color backgroundColor;
    sf::Color textColor;
    sf::Color buttonColor;
    sf::Color outlineColor;
    sf::Color hoverColor;
    sf::Color disabledColor;
};

enum class Screen {
    LANGUAGE_SELECTION,
    SECOND_SCREEN
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1100, 600), "Language Selection", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    // Definir los temas
    Theme darkTheme = {
        sf::Color(32, 33, 35),     // Fondo
        sf::Color(224, 224, 224),  // Texto
        sf::Color(60, 60, 60),     // Botones
        sf::Color(100, 100, 100),  // Borde
        sf::Color(80, 80, 80),     // Hover
        sf::Color(40, 40, 40)      // Desactivado
    };

    Theme lightTheme = {
        sf::Color(220, 220, 220),  // Fondo
        sf::Color(50, 50, 50),     // Texto
        sf::Color(200, 200, 200),  // Botones
        sf::Color(150, 150, 150),  // Borde
        sf::Color(180, 180, 180),  // Hover
        sf::Color(160, 160, 160)   // Desactivado
    };


    bool isDarkTheme = true;
    Theme currentTheme = darkTheme;

    // Cargar fuente
    sf::Font font;
    if (!font.loadFromFile("../fonts/Ubuntu-Regular.ttf")) {
        std::cerr << "Error al cargar la fuente" << std::endl;
        return -1;
    }

    // Cargar texturas para el botón de tema
    sf::Texture themeIconTexture;
    if (!themeIconTexture.loadFromFile("../images/theme_icon.png")) {
        std::cerr << "Error al cargar el icono de tema" << std::endl;
        return -1;
    }
    sf::Sprite themeIcon(themeIconTexture);

    //Theme button
    sf::RectangleShape themeButton(sf::Vector2f(40, 40)); // Size
    themeButton.setPosition(25, 25);
    themeButton.setFillColor(currentTheme.buttonColor);
    themeButton.setOutlineThickness(1);
    themeButton.setOutlineColor(currentTheme.outlineColor);

    // Centrar el icono en el botón
    float desiredSize = 80.0f;
    float scale = desiredSize / std::max(themeIcon.getLocalBounds().width, 
                                       themeIcon.getLocalBounds().height);
    themeIcon.setScale(scale, scale);
    
    // Centrar el icono en el botón
    themeIcon.setPosition(
        themeButton.getPosition().x + (themeButton.getSize().x - themeIcon.getGlobalBounds().width)/2,
        themeButton.getPosition().y + (themeButton.getSize().y - themeIcon.getGlobalBounds().height)/2
    );

    // Texto principal
    sf::Text title("Select a language", font, 30);
    title.setPosition(430, 60);
    title.setFillColor(currentTheme.textColor);

    // Botones de banderas
    sf::RectangleShape buttonCR(sf::Vector2f(200, 120));
    buttonCR.setPosition(150, 200);
    buttonCR.setFillColor(currentTheme.buttonColor);
    buttonCR.setOutlineThickness(2);
    buttonCR.setOutlineColor(currentTheme.outlineColor);

    sf::RectangleShape buttonUSA(sf::Vector2f(200, 120));
    buttonUSA.setPosition(450, 200);
    buttonUSA.setFillColor(currentTheme.buttonColor);
    buttonUSA.setOutlineThickness(2);
    buttonUSA.setOutlineColor(currentTheme.outlineColor);

    sf::RectangleShape buttonBrasil(sf::Vector2f(200, 120));
    buttonBrasil.setPosition(750, 200);
    buttonBrasil.setFillColor(currentTheme.buttonColor);
    buttonBrasil.setOutlineThickness(2);
    buttonBrasil.setOutlineColor(currentTheme.outlineColor);

    // Cargar texturas de banderas
    sf::Texture flagTextureCR, flagTextureUSA, flagTextureBrasil;
    if (!flagTextureCR.loadFromFile("../images/costa_rica_flag.png") || 
        !flagTextureUSA.loadFromFile("../images/usa_flag.png") ||
        !flagTextureBrasil.loadFromFile("../images/brasil_flag.png")) {
        std::cerr << "Error al cargar las imágenes de banderas" << std::endl;
        return -1;
    }

    // Sprites de banderas
    sf::Sprite flagSpriteCR(flagTextureCR), flagSpriteUSA(flagTextureUSA), flagSpriteBrasil(flagTextureBrasil);
    
    auto scaleAndCenterFlag = [](sf::Sprite& sprite, sf::RectangleShape& button) {
        float scale = std::min(
            button.getSize().x / sprite.getLocalBounds().width,
            button.getSize().y / sprite.getLocalBounds().height
        );
        sprite.setScale(scale, scale);
        sprite.setPosition(
            button.getPosition().x + (button.getSize().x - sprite.getGlobalBounds().width)/2,
            button.getPosition().y + (button.getSize().y - sprite.getGlobalBounds().height)/2
        );
    };

    scaleAndCenterFlag(flagSpriteCR, buttonCR);
    scaleAndCenterFlag(flagSpriteUSA, buttonUSA);
    scaleAndCenterFlag(flagSpriteBrasil, buttonBrasil);

    // Texto de selección
    sf::Text selectionText("", font, 24);
    selectionText.setFillColor(currentTheme.textColor);
    selectionText.setStyle(sf::Text::Bold);

    // Botón NEXT
    sf::RectangleShape nextButton(sf::Vector2f(200, 50));
    nextButton.setPosition((1100 - 200)/2, 480);
    nextButton.setFillColor(currentTheme.disabledColor);
    nextButton.setOutlineThickness(2);
    nextButton.setOutlineColor(currentTheme.outlineColor);

    sf::Text nextText("Next", font, 24);
    nextText.setFillColor(sf::Color(150, 150, 150));
    nextText.setPosition(
        nextButton.getPosition().x + (nextButton.getSize().x - nextText.getLocalBounds().width)/2,
        nextButton.getPosition().y + (nextButton.getSize().y - nextText.getLocalBounds().height)/2 - 5
    );

    // Variables de estado
    bool languageSelected = false;
    bool mouseOnNextButton = false;
    bool mouseOnThemeButton = false;
    std::string selectedLanguage = "";

    // Bucle principal
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Manejo de eventos del ratón
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                mouseOnThemeButton = themeButton.getGlobalBounds().contains(mousePos);
                mouseOnNextButton = nextButton.getGlobalBounds().contains(mousePos) && languageSelected;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    
                    if (themeButton.getGlobalBounds().contains(mousePos)) {
                        isDarkTheme = !isDarkTheme;
                        currentTheme = isDarkTheme ? darkTheme : lightTheme;
                        
                        // Actualizar todos los elementos con el nuevo tema
                        title.setFillColor(currentTheme.textColor);
                        selectionText.setFillColor(currentTheme.textColor);
                        
                        buttonCR.setFillColor(currentTheme.buttonColor);
                        buttonCR.setOutlineColor(currentTheme.outlineColor);
                        buttonUSA.setFillColor(currentTheme.buttonColor);
                        buttonUSA.setOutlineColor(currentTheme.outlineColor);
                        buttonBrasil.setFillColor(currentTheme.buttonColor);
                        buttonBrasil.setOutlineColor(currentTheme.outlineColor);
                        
                        themeButton.setFillColor(mouseOnThemeButton ? currentTheme.hoverColor : currentTheme.buttonColor);
                        themeButton.setOutlineColor(currentTheme.outlineColor);
                        
                        nextButton.setFillColor(languageSelected ? currentTheme.buttonColor : currentTheme.disabledColor);
                        nextButton.setOutlineColor(currentTheme.outlineColor);
                        nextText.setFillColor(languageSelected ? currentTheme.textColor : sf::Color(150, 150, 150));
                    }
                    else if (buttonCR.getGlobalBounds().contains(mousePos)) {
                        selectionText.setString("Idioma seleccionado: Espanol");
                        selectionText.setPosition(390, 350);
                        selectedLanguage = "Spanish";
                        languageSelected = true;
                        nextButton.setFillColor(currentTheme.buttonColor);
                        nextText.setFillColor(currentTheme.textColor);
                    }
                    else if (buttonUSA.getGlobalBounds().contains(mousePos)) {
                        selectionText.setString("Selected language: English");
                        selectionText.setPosition(400, 350);
                        selectedLanguage = "English";
                        languageSelected = true;
                        nextButton.setFillColor(currentTheme.buttonColor);
                        nextText.setFillColor(currentTheme.textColor);
                    }
                    else if (buttonBrasil.getGlobalBounds().contains(mousePos)) {
                        selectionText.setString("Idioma selecionado: Portugues");
                        selectionText.setPosition(385, 350);
                        selectedLanguage = "Portuguese";
                        languageSelected = true;
                        nextButton.setFillColor(currentTheme.buttonColor);
                        nextText.setFillColor(currentTheme.textColor);
                    }
                }
            }
        }

        // Actualizar colores de hover
        themeButton.setFillColor(mouseOnThemeButton ? currentTheme.hoverColor : currentTheme.buttonColor);
        if (languageSelected) {
            nextButton.setFillColor(mouseOnNextButton ? currentTheme.hoverColor : currentTheme.buttonColor);
        }

        // Dibujado
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
        window.draw(nextText);
        window.draw(themeButton);
        window.draw(themeIcon); // Dibujar el icono del tema
        window.display();
    }

    return 0;
}