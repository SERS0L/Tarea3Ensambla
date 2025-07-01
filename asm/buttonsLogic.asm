section .text

;font size functions
global increaseFontSize
global decreaseFontSize
global changeTheme

; drak theme functions
global getDarkThemeBackgroundColor
global getDarkThemeTextColor
global getDarkThemeButtonColor
global getDarkThemeOutlineColor
global getDarkThemeHoverColor
global getDarkThemeDisabledColor

; light Theme functions
global getLightThemeBackgroundColor
global getLightThemeTextColor
global getLightThemeButtonColor
global getLightThemeOutlineColor
global getLightThemeHoverColor
global getLightThemeDisabledColor

increaseFontSize:
    ; int increaseFontSize(int currentFontSize)
    ; edi -> currentSize
    mov eax, edi
    cmp eax, 100
    jge .done
    add eax, 2
.done:
    ret

decreaseFontSize:
    ; int decreaseFontSize(int currentFontSize)
    ; edi -> currentSize
    mov eax, edi
    cmp eax, 10
    jle .done
    sub eax, 2
.done:
    ret

changeTheme:
    ;int changeTheme(int currentTheme)
    ; edi -> currentTheme
    mov eax, edi
    xor eax, 1 ;Toggle theme
    ret

getDarkThemeBackgroundColor:
    mov eax, 0x202123 ; Dark background color
    ret

getDarkThemeTextColor:
    mov eax, 0xE0E0E0 ; Light text color
    ret

getDarkThemeButtonColor:
    mov eax, 0x3C3C3C ; Dark button color
    ret

getDarkThemeOutlineColor:
    mov eax, 0x646464 ; Dark outline color
    ret

getDarkThemeHoverColor:
    mov eax, 0x646464 ; Dark hover color
    ret

getDarkThemeDisabledColor:
    mov eax, 0x282828 ; Dark disabled color
    ret

getLightThemeBackgroundColor:
    mov eax, 0xD2D2D2 ; Light background color
    ret

getLightThemeTextColor:
    mov eax, 0x323232 ; Dark text color
    ret

getLightThemeButtonColor:
    mov eax, 0xC8C8C8 ; Light button color
    ret

getLightThemeOutlineColor:
    mov eax, 0x969696 ; Light outline color
    ret

getLightThemeHoverColor:
    mov eax, 0xB4B4B4 ; Light hover color
    ret

getLightThemeDisabledColor:
    mov eax, 0xA0A0A0 ; Light disabled color
    ret