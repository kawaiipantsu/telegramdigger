#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <iostream>

namespace TelegramDigger {

/**
 * Terminal styling and color utilities
 * Supports 256-color ANSI codes and UTF-8 icons
 */
class Terminal {
public:
    // ANSI Color codes (256-color support)
    enum class Color {
        RESET = 0,
        BLACK = 0,
        RED = 1,
        GREEN = 2,
        YELLOW = 3,
        BLUE = 4,
        MAGENTA = 5,
        CYAN = 6,
        WHITE = 7,
        BRIGHT_BLACK = 8,
        BRIGHT_RED = 9,
        BRIGHT_GREEN = 10,
        BRIGHT_YELLOW = 11,
        BRIGHT_BLUE = 12,
        BRIGHT_MAGENTA = 13,
        BRIGHT_CYAN = 14,
        BRIGHT_WHITE = 15
    };

    // Geek Font Icons (UTF-8)
    static constexpr const char* ICON_TELEGRAM = "\uf2c6";      //
    static constexpr const char* ICON_SEARCH = "\uf002";        //
    static constexpr const char* ICON_INFO = "\uf05a";          //
    static constexpr const char* ICON_WARNING = "\uf071";       //
    static constexpr const char* ICON_SUCCESS = "\uf00c";       //
    static constexpr const char* ICON_ERROR = "\uf00d";         //
    static constexpr const char* ICON_CONFIG = "\uf013";        //
    static constexpr const char* ICON_KEY = "\uf084";           //
    static constexpr const char* ICON_LOCK = "\uf023";          //
    static constexpr const char* ICON_UNLOCK = "\uf09c";        //

    /**
     * Set foreground color (256-color mode)
     */
    static std::string fg(int colorCode);

    /**
     * Set foreground color using Color enum
     */
    static std::string fg(Color color);

    /**
     * Set background color (256-color mode)
     */
    static std::string bg(int colorCode);

    /**
     * Set background color using Color enum
     */
    static std::string bg(Color color);

    /**
     * Reset all terminal attributes
     */
    static std::string reset();

    /**
     * Bold text
     */
    static std::string bold();

    /**
     * Dim text
     */
    static std::string dim();

    /**
     * Underline text
     */
    static std::string underline();

    /**
     * Clear screen
     */
    static void clearScreen();

    /**
     * Check if terminal supports UTF-8
     */
    static bool supportsUTF8();

    /**
     * Check if terminal supports 256 colors
     */
    static bool supports256Colors();

    /**
     * Colorize text with foreground color
     */
    static std::string colorize(const std::string& text, Color color);

    /**
     * Print colored text
     */
    static void print(const std::string& text, Color color = Color::RESET);

    /**
     * Print error message
     */
    static void error(const std::string& message);

    /**
     * Print success message
     */
    static void success(const std::string& message);

    /**
     * Print warning message
     */
    static void warning(const std::string& message);

    /**
     * Print info message
     */
    static void info(const std::string& message);
};

} // namespace TelegramDigger

#endif // TERMINAL_H
