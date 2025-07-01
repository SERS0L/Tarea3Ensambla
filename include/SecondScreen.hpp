#ifndef SECOND_SCREEN_HPP
#define SECOND_SCREEN_HPP

#include "Theme.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

class SecondScreen {
public:
    SecondScreen(sf::RenderWindow& win, const std::string& language, const Theme& theme, unsigned int fontSize, bool isBold);

    void handleEvent(const sf::Event& event);
    void update();
    void draw();

    unsigned int getSelectedFontSize() const;
    std::string getLastPrompt() const;

private:
    // TTS
    void speakText(const std::string& text);
    // Transcription
    std::string transcribeAudio();
    std::string getLanguageCode() const;

    // General Config
    sf::RenderWindow& window;
    Theme currentTheme;
    std::string selectedLanguage;
    unsigned int fontSize;
    bool isBold;

    // Audio
    bool isRecording;
    sf::Clock recordingClock;
    sf::SoundBufferRecorder* recorder;

    // Font
    sf::Font font;

    // Visual Components
    sf::Text prompt;
    sf::Text inputText;
    sf::Text responseText;

    sf::RectangleShape micButton;
    sf::RectangleShape inputField;
    sf::RectangleShape responseBox;

    sf::Texture micIconTexture;
    sf::Texture micRecordingTexture;
    sf::Sprite micIcon;

    // User
    std::string userInput;
    std::string lastSubmittedPrompt;
};

#endif

    
