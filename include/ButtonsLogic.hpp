#pragma once

extern "C" {
    // font size functions
    unsigned int increaseFontSize(unsigned int currentFontSize);
    unsigned int decreaseFontSize(unsigned int currentFontSize);
    int changeTheme(int currentTheme);
    // dark theme functions
    unsigned int getDarkThemeBackgroundColor();
    unsigned int getDarkThemeTextColor();
    unsigned int getDarkThemeButtonColor();
    unsigned int getDarkThemeOutlineColor();
    unsigned int getDarkThemeHoverColor();
    unsigned int getDarkThemeDisabledColor();
    //light theme functions
    unsigned int getLightThemeBackgroundColor();
    unsigned int getLightThemeTextColor();
    unsigned int getLightThemeButtonColor();
    unsigned int getLightThemeOutlineColor();
    unsigned int getLightThemeHoverColor();
    unsigned int getLightThemeDisabledColor();
}