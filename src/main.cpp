#include <SFML/Graphics.hpp>
#include "../include/LanguageScreen.hpp"
#include "../include/SecondScreen.hpp"

enum class Screen {
    LANGUAGE_SELECTION,
    SECOND_SCREEN
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1100, 600), "Language Selection", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    Screen currentScreen = Screen::LANGUAGE_SELECTION;

    LanguageScreen langScreen(window);
    SecondScreen* secondScreen = nullptr;  // Usamos puntero porque se crea más tarde

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Delegamos eventos según pantalla actual
            if (currentScreen == Screen::LANGUAGE_SELECTION) {
                langScreen.handleEvent(event);
            } else if (currentScreen == Screen::SECOND_SCREEN && secondScreen) {
                secondScreen->handleEvent(event);
            }
        }

        // Lógica de cambio de pantalla
        if (currentScreen == Screen::LANGUAGE_SELECTION && langScreen.shouldSwitchScreen()) {
            std::string language = langScreen.getSelectedLanguage();
            secondScreen = new SecondScreen(window, language);  // Ahora pasamos el idioma aquí
            currentScreen = Screen::SECOND_SCREEN;
        }

        // Update & draw según pantalla
        window.clear();

        if (currentScreen == Screen::LANGUAGE_SELECTION) {
            langScreen.update();
            langScreen.draw();
        } else if (currentScreen == Screen::SECOND_SCREEN && secondScreen) {
            secondScreen->update();
            secondScreen->draw();
        }

        window.display();
    }

    delete secondScreen; // Liberar memoria del new
    return 0;
}
