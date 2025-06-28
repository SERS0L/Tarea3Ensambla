#include "../include/SecondScreen.hpp"
#include <curl/curl.h>
#include <iostream>

SecondScreen::SecondScreen(sf::RenderWindow& win, const std::string& language)
    : window(win), selectedLanguage(language) {

    if (!font.loadFromFile("resources/fonts/Ubuntu-Regular.ttf")) {
        std::cerr << "Error cargando fuente" << std::endl;
        exit(1);
    }

    if (!micTexture.loadFromFile("resources/images/mic_icon.png")) {
        std::cerr << "Error cargando icono del microfono" << std::endl;
        exit(1);
    }

    // Texto superior con el idioma
    label.setFont(font);
    label.setString("Idioma seleccionado: " + selectedLanguage);
    label.setCharacterSize(28);
    label.setFillColor(sf::Color::White);
    label.setPosition(300, 60);

    // Botón de micrófono
    micButton.setSize({80, 80});
    micButton.setFillColor(sf::Color(100, 100, 100));
    micButton.setPosition(510, 200);
    micButton.setOutlineThickness(2);
    micButton.setOutlineColor(sf::Color::White);

    micSprite.setTexture(micTexture);
    float scale = 60.0f / std::max(micSprite.getLocalBounds().width, micSprite.getLocalBounds().height);
    micSprite.setScale(scale, scale);
    micSprite.setPosition(
        micButton.getPosition().x + (micButton.getSize().x - micSprite.getGlobalBounds().width) / 2,
        micButton.getPosition().y + (micButton.getSize().y - micSprite.getGlobalBounds().height) / 2
    );

    // Caja de texto
    textBox.setSize({400, 50});
    textBox.setFillColor(sf::Color(30, 30, 30));
    textBox.setOutlineThickness(2);
    textBox.setOutlineColor(sf::Color::White);
    textBox.setPosition(350, 320);

    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(textBox.getPosition().x + 10, textBox.getPosition().y + 10);

    answerText.setFont(font);
    answerText.setCharacterSize(18);
    answerText.setFillColor(sf::Color::White);
    answerText.setPosition(350, 400);  //debajo del inputText

}

void SecondScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        isFocused = textBox.getGlobalBounds().contains(mousePos);
    }

    if (isFocused && event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) { // Backspace
            if (!currentInput.empty())
                currentInput.pop_back();
        } else if (event.text.unicode >= 32 && event.text.unicode <= 126) {
            currentInput += static_cast<char>(event.text.unicode);
        }
        inputText.setString(currentInput);
    }

    if (event.text.unicode == 13 && !currentInput.empty()) {
        std::string response = sendQuestionToBackend(currentInput);
        answerText.setString("Respuesta: " + response);
    }
}

void SecondScreen::update() {
    // Futuro: hover o animaciones del micrófono
}

void SecondScreen::draw() {
    window.clear(sf::Color(40, 40, 40));
    window.draw(label);
    window.draw(micButton);
    window.draw(micSprite);
    window.draw(textBox);
    window.draw(inputText);
    window.draw(answerText);
}

std::string sendQuestionToBackend(const std::string& question) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::string jsonData = "{\"question\":\"" + question + "\"}";

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/ask");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
            [](char* ptr, size_t size, size_t nmemb, void* userdata) -> size_t {
                std::string* str = static_cast<std::string*>(userdata);
                size_t totalSize = size * nmemb;
                str->append(ptr, totalSize);
                return totalSize;
            });

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    return readBuffer;
}
