#include "../include/Theme.hpp"
#include "../include/ButtonsLogic.hpp"

sf::Color uintToColor(unsigned int value) {
    return sf::Color(
        (value >> 16) & 0xFF,  // R
        (value >> 8) & 0xFF,   // G
        value & 0xFF,          // B
        255                    // A (opacidad completa)
    );
}

Theme createDarkTheme() {
    return Theme{
        uintToColor(getDarkThemeBackgroundColor()),
        uintToColor(getDarkThemeTextColor()),
        uintToColor(getDarkThemeButtonColor()),
        uintToColor(getDarkThemeOutlineColor()),
        uintToColor(getDarkThemeHoverColor()),
        uintToColor(getDarkThemeDisabledColor())
    };
}

Theme createLightTheme() {
    return Theme{
        uintToColor(getLightThemeBackgroundColor()),
        uintToColor(getLightThemeTextColor()),
        uintToColor(getLightThemeButtonColor()),
        uintToColor(getLightThemeOutlineColor()),
        uintToColor(getLightThemeHoverColor()),
        uintToColor(getLightThemeDisabledColor())
    };
}