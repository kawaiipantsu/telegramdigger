#include "terminal.h"
#include <cstdlib>
#include <cstring>
#include <sstream>

namespace TelegramDigger {

std::string Terminal::fg(int colorCode) {
    std::ostringstream oss;
    oss << "\033[38;5;" << colorCode << "m";
    return oss.str();
}

std::string Terminal::fg(Color color) {
    return fg(static_cast<int>(color));
}

std::string Terminal::bg(int colorCode) {
    std::ostringstream oss;
    oss << "\033[48;5;" << colorCode << "m";
    return oss.str();
}

std::string Terminal::bg(Color color) {
    return bg(static_cast<int>(color));
}

std::string Terminal::reset() {
    return "\033[0m";
}

std::string Terminal::bold() {
    return "\033[1m";
}

std::string Terminal::dim() {
    return "\033[2m";
}

std::string Terminal::underline() {
    return "\033[4m";
}

void Terminal::clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

bool Terminal::supportsUTF8() {
    const char* lang = std::getenv("LANG");
    if (lang) {
        std::string langStr(lang);
        return langStr.find("UTF-8") != std::string::npos ||
               langStr.find("utf8") != std::string::npos;
    }
    return false;
}

bool Terminal::supports256Colors() {
    const char* term = std::getenv("TERM");
    if (term) {
        std::string termStr(term);
        return termStr.find("256color") != std::string::npos;
    }
    return false;
}

std::string Terminal::colorize(const std::string& text, Color color) {
    return fg(color) + text + reset();
}

void Terminal::print(const std::string& text, Color color) {
    if (color != Color::RESET) {
        std::cout << fg(color);
    }
    std::cout << text;
    if (color != Color::RESET) {
        std::cout << reset();
    }
}

void Terminal::error(const std::string& message) {
    std::cout << fg(Color::BRIGHT_RED) << bold();
    if (supportsUTF8()) {
        std::cout << ICON_ERROR << " ";
    }
    std::cout << "Error: " << reset() << fg(Color::RED) << message << reset() << std::endl;
}

void Terminal::success(const std::string& message) {
    std::cout << fg(Color::BRIGHT_GREEN) << bold();
    if (supportsUTF8()) {
        std::cout << ICON_SUCCESS << " ";
    }
    std::cout << reset() << fg(Color::GREEN) << message << reset() << std::endl;
}

void Terminal::warning(const std::string& message) {
    std::cout << fg(Color::BRIGHT_YELLOW) << bold();
    if (supportsUTF8()) {
        std::cout << ICON_WARNING << " ";
    }
    std::cout << "Warning: " << reset() << fg(Color::YELLOW) << message << reset() << std::endl;
}

void Terminal::info(const std::string& message) {
    std::cout << fg(Color::BRIGHT_CYAN) << bold();
    if (supportsUTF8()) {
        std::cout << ICON_INFO << " ";
    }
    std::cout << reset() << fg(Color::CYAN) << message << reset() << std::endl;
}

} // namespace TelegramDigger
