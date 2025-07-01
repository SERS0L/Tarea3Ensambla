#include "../include/SecondScreen.hpp"
#include "../include/Utils.hpp"
#include "../include/json.hpp"

#include <chrono>
#include <thread>
#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <iostream>



static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
  }
  
  std::string sendPromptToBackend(const std::string& prompt) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
  
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
  
    if (curl) {
        nlohmann::json j;
        j["prompt"] = prompt;
        j["model"] = "llama3";
        j["language"] = "ensamblador";
        std::string jsonStr = j.dump();
  
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
  
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/api/assembly/recognizer");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
  
        res = curl_easy_perform(curl);
  
        if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
  
      curl_easy_cleanup(curl);
      curl_slist_free_all(headers);
    }
  
    curl_global_cleanup();
    return readBuffer;
}

std::string wrapText(const std::string& text, const sf::Font& font, unsigned int charSize, float maxWidth) {
    std::istringstream iss(text);
    std::string word, line, output;
    sf::Text tempText("", font, charSize);

    while (iss >> word) {
        std::string testLine = line.empty() ? word : line + " " + word;
        tempText.setString(testLine);

        if (tempText.getLocalBounds().width > maxWidth) {
            output += line + "\n";
            line = word;
        } else {
            line = testLine;
        }
    }

    if (!line.empty()) {
        output += line;
    }

    return output;
}

void SecondScreen::speakText(const std::string& text) {
    std::string langCode;

    if (selectedLanguage == "Spanish") langCode = "es";
    else if (selectedLanguage == "Portuguese") langCode = "pt";
    else langCode = "en";

    std::string command = "espeak -v " + langCode + " \"" + text + "\"";
    std::system(command.c_str());
}


SecondScreen::SecondScreen(sf::RenderWindow& win, const std::string& language, const Theme& theme, unsigned int fontSize, bool isBold)
    : window(win), selectedLanguage(language), isRecording(false), recorder(nullptr),
      currentTheme(theme), fontSize(fontSize), isBold(isBold) {

    if (!font.loadFromFile("resources/fonts/Ubuntu-Regular.ttf")) {
        std::cerr << "There has been an error loading the font" << std::endl;
        exit(1);
    }

    if (!micIconTexture.loadFromFile("resources/images/mic_icon.png") ||
        !micRecordingTexture.loadFromFile("resources/images/mic_recording.png")) {
        std::cerr << "There has been an error loading the mic images" << std::endl;
        exit(1);
    }

    micButton.setSize({80, 80});
    micButton.setPosition((window.getSize().x - 80) / 2, 200);
    micIcon.setTexture(micIconTexture);
    scaleAndCenter(micIcon, micButton);

    inputField.setSize({400, 40});
    inputField.setFillColor(sf::Color::White);
    inputField.setPosition((window.getSize().x - 400) / 2, 300);

    responseBox.setSize({600, 500});
    responseBox.setFillColor(sf::Color::White);
    responseBox.setOutlineColor(sf::Color::Black);
    responseBox.setOutlineThickness(1);
    responseBox.setPosition((window.getSize().x - 600) / 2, 370); 

    prompt.setFont(font);
    prompt.setCharacterSize(fontSize + 4);
    prompt.setFillColor(currentTheme.textColor);
    prompt.setPosition(450, 100);

    inputText.setFont(font);
    inputText.setCharacterSize(fontSize);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(inputField.getPosition().x + 10, inputField.getPosition().y + 5);

    responseText.setFont(font);
    responseText.setCharacterSize(fontSize);
    responseText.setFillColor(sf::Color::Black);
    responseText.setPosition(responseBox.getPosition().x + 10, responseBox.getPosition().y + 10);

    if (isBold) {
        prompt.setStyle(sf::Text::Bold);
        inputText.setStyle(sf::Text::Bold);
        responseText.setStyle(sf::Text::Bold);
    }

    if (selectedLanguage == "Spanish") {
        prompt.setString("Presiona el botón para grabar tu voz o escribe a continuación:");
    } else if (selectedLanguage == "Portuguese") {
        prompt.setString("Pressione o botão para gravar sua voz ou digite abaixo:");
    } else {
        prompt.setString("Press the button to record your voice or type below:");
    }
}

void SecondScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
        if (micButton.getGlobalBounds().contains(pos)) {
            if (!isRecording) {
                if (!sf::SoundRecorder::isAvailable()) {
                    std::cerr << "Recording is not available" << std::endl;
                    return;
                }
                recorder = new sf::SoundBufferRecorder();
                recorder->start();
                recordingClock.restart();
                isRecording = true;
                micIcon.setTexture(micRecordingTexture);
            } else {
                recorder->stop();
                sf::SoundBuffer buffer = recorder->getBuffer();
                buffer.saveToFile("resources/audio/grabacion.wav");
                delete recorder;
                recorder = nullptr;
                isRecording = false;
                micIcon.setTexture(micIconTexture);

                // Transcribe the audio with Whisper CLI
                std::string transcription = transcribeAudio();
                userInput = transcription;
                inputText.setString(userInput); // Update the input text with the transcription
                std::string responseJson = sendPromptToBackend(userInput);
                try {
                    auto parsed = nlohmann::json::parse(responseJson);
                    std::string modelResponse = parsed["data"]["response"];
                    std::string wrappedResponse = wrapText(modelResponse, font, fontSize, responseBox.getSize().x - 20);
                    responseText.setString(wrappedResponse);
                    speakText(modelResponse); // Speaks the response
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                    responseText.setString("There has been an error parsing the response.");
                }
                
            }
        }
    } else if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8 && !userInput.empty()) { // Backspace
            userInput.pop_back();
        } else if (event.text.unicode == 13) { // Enter
            // Saves the prompt when Enter is pressed
            lastSubmittedPrompt = userInput;
            userInput.clear();
            inputText.setString("");  // Clear the input field
            std::string responseJson = sendPromptToBackend(lastSubmittedPrompt);
            try {
                auto parsed = nlohmann::json::parse(responseJson);
                std::string modelResponse = parsed["data"]["response"];
                std::string wrappedResponse = wrapText(modelResponse, font, fontSize, responseBox.getSize().x - 20);
                responseText.setString(wrappedResponse);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
                responseText.setString("There has been an error parsing the response.");
            }
        } else if (event.text.unicode < 128 && event.text.unicode >= 32) {
            userInput += static_cast<char>(event.text.unicode);
            inputText.setString(userInput); // Shows what the user is typing
        }
    }
}

void SecondScreen::update() {
    if (isRecording) {
        int seconds = static_cast<int>(recordingClock.getElapsedTime().asSeconds());
        std::string recordingMsg;
        if (selectedLanguage == "Spanish")
            recordingMsg = "Grabando: " + std::to_string(seconds) + "s";
        else if (selectedLanguage == "Portuguese")
            recordingMsg = "Gravando: " + std::to_string(seconds) + "s";
        else
            recordingMsg = "Recording: " + std::to_string(seconds) + "s";
        prompt.setString(recordingMsg);
    }

    inputText.setString(userInput); // Update the input text with the current user input
}

std::string SecondScreen::getLastPrompt() const {
    return lastSubmittedPrompt;
}

std::string SecondScreen::getLanguageCode() const {
    if (selectedLanguage == "Spanish") return "es";
    if (selectedLanguage == "Portuguese") return "pt";
    return "en";
}

std::string SecondScreen::transcribeAudio() {
    std::string command = "./whisper/whisper-cli"
                          " -m whisper/models/ggml-base.en.bin"
                          " -f resources/audio/grabacion.wav"
                          " --output-txt"
                          " --output-file resources/questions/grabacion"
                          " -l " + getLanguageCode();

    int result = std::system(command.c_str());

    if (result != 0) {
        std::cerr << "Error al ejecutar whisper-cli" << std::endl;
        return "";
    }

    std::ifstream transcript("resources/questions/grabacion.txt");
    if (!transcript.is_open()) {
        std::cerr << "Could not open transcription file" << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << transcript.rdbuf();
    transcript.close();

    return buffer.str();
}

void SecondScreen::draw() {
    window.clear(currentTheme.backgroundColor);
    window.draw(prompt);
    window.draw(micButton);
    window.draw(micIcon);
    window.draw(inputField);
    window.draw(inputText);
    window.draw(responseBox);
    window.draw(responseText);
    window.display();
}