#include "terminal.h"
#include "config.h"
#include "version.h"
#include "telegram_api.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

using namespace TelegramDigger;

/**
 * Display help screen
 */
void showHelp() {
    Terminal::clearScreen();

    // Header with ASCII art - Compact shovel digging in dirt (6 lines)
    std::cout << "\n";

    // Line 1: Shovel handle
    std::cout << "       " << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << "___" << Terminal::fg(Terminal::Color::YELLOW) << "/" << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << "\\" << Terminal::reset();
    std::cout << "   " << Terminal::fg(Terminal::Color::BRIGHT_RED) << Terminal::bold() << "TELEGRAM DIGGER" << Terminal::reset();
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << " by " << Terminal::fg(Terminal::Color::BRIGHT_MAGENTA) << "THUGSred" << Terminal::reset() << "\n";

    // Line 2: Shovel blade top
    std::cout << "      " << Terminal::fg(Terminal::Color::YELLOW) << "/" << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << "     " << Terminal::fg(Terminal::Color::YELLOW) << "\\" << Terminal::reset();
    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_WHITE) << "Security Testing Tool for Telegram Bot Tokens" << Terminal::reset() << "\n";

    // Line 3: Shovel blade in dirt
    std::cout << "   " << Terminal::fg(Terminal::Color::YELLOW) << "___";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << Terminal::bold() << ".#" << Terminal::fg(Terminal::Color::YELLOW) << "===" << Terminal::reset();
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "___" << Terminal::reset() << "\n";

    // Line 4: Dirt pile top
    std::cout << "  " << Terminal::fg(Terminal::Color::YELLOW) << "/";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << Terminal::bold() << "..#";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "##";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << "#.";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "###";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << "##.#";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "\\" << Terminal::reset() << "\n";

    // Line 5: Dirt pile middle
    std::cout << " " << Terminal::fg(Terminal::Color::YELLOW) << "/";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << Terminal::bold() << "#";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "##";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << ".";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "###";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << "#";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "##";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << "..";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "###";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << "#.";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "\\" << Terminal::reset() << "\n";

    // Line 6: Dirt pile bottom
    std::cout << " " << Terminal::fg(Terminal::Color::YELLOW) << "\\";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << Terminal::bold() << ".";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "###";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << "..#";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "##";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << "#";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "###";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << "#.";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "##";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << ".";
    std::cout << Terminal::fg(Terminal::Color::YELLOW) << "/" << Terminal::reset() << "\n\n";

    // Description
    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "A tool for exploring and testing Telegram bot tokens discovered during\n";
    std::cout << "security assessments. Supports read/write operations to the Bot API and\n";
    std::cout << "analysis of bot token security.\n\n";
    std::cout << Terminal::reset();

    // Usage section
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << Terminal::bold() << "USAGE:\n" << Terminal::reset();
    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_WHITE) << "telegramdigger"
              << Terminal::fg(Terminal::Color::WHITE) << " [OPTIONS] [COMMAND]\n\n" << Terminal::reset();

    // Options section
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << Terminal::bold() << "OPTIONS:\n" << Terminal::reset();

    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << "--help"
              << Terminal::fg(Terminal::Color::WHITE) << "              Show this help message\n";

    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << "--version"
              << Terminal::fg(Terminal::Color::WHITE) << "           Show version information\n";

    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << "--about"
              << Terminal::fg(Terminal::Color::WHITE) << "             Show about information\n";

    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << "--token"
              << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << " <TOKEN>"
              << Terminal::fg(Terminal::Color::WHITE) << "      Set bot token for operations\n";

    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << "--validate"
              << Terminal::fg(Terminal::Color::WHITE) << "          Validate bot token via API\n";

    std::cout << Terminal::reset() << std::endl;

    // Environment variables section
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << Terminal::bold() << "ENVIRONMENT:\n" << Terminal::reset();
    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_MAGENTA) << "TGDIGGER_TOKEN"
              << Terminal::fg(Terminal::Color::WHITE) << "     Bot token (alternative to --token)\n";

    std::cout << Terminal::reset() << std::endl;

    // Commands section (placeholders for future implementation)
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << Terminal::bold() << "COMMANDS:\n" << Terminal::reset();
    std::cout << Terminal::dim() << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << "  [Coming soon]\n";
    std::cout << "  scan                Scan for bot token capabilities\n";
    std::cout << "  info                Get bot information\n";
    std::cout << "  test                Test bot token validity\n";
    std::cout << "  analyze             Analyze bot permissions and settings\n";
    std::cout << Terminal::reset() << std::endl;

    // Configuration section
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << Terminal::bold() << "CONFIGURATION:\n" << Terminal::reset();
    std::cout << "  Config directory:   " << Terminal::fg(Terminal::Color::BRIGHT_CYAN);
    if (Terminal::supportsUTF8()) {
        std::cout << Terminal::ICON_CONFIG << " ";
    }
    std::cout << Config::getInstance().getConfigDir() << "\n";
    std::cout << Terminal::reset();
    std::cout << "  Config file:        " << Terminal::fg(Terminal::Color::CYAN)
              << Config::getInstance().getConfigFile() << "\n";
    std::cout << Terminal::reset() << std::endl;

    // Security notice
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_RED) << Terminal::bold();
    if (Terminal::supportsUTF8()) {
        std::cout << Terminal::ICON_WARNING << " ";
    }
    std::cout << "SECURITY NOTICE:\n" << Terminal::reset();
    std::cout << Terminal::fg(Terminal::Color::YELLOW);
    std::cout << "This tool is intended for authorized security testing, CTF challenges,\n";
    std::cout << "and educational contexts only. Use only with bot tokens you own or have\n";
    std::cout << "explicit permission to test.\n";
    std::cout << Terminal::reset() << std::endl;

    // Footer
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK);
    std::cout << "For more information, visit: https://github.com/kawaiipantsu/telegramdigger\n";
    std::cout << Terminal::reset();
}

/**
 * Display version information
 */
void showVersion() {
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << Terminal::bold();

    if (Terminal::supportsUTF8()) {
        std::cout << Terminal::ICON_TELEGRAM << " ";
    }

    std::cout << APP_NAME << Terminal::reset()
              << Terminal::fg(Terminal::Color::WHITE) << " v" << VERSION << "\n";
    std::cout << Terminal::reset();

    // Terminal capabilities
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK);
    std::cout << "\nTerminal capabilities:\n";
    std::cout << "  UTF-8 support:      " << (Terminal::supportsUTF8() ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";
    std::cout << "  256-color support:  " << (Terminal::supports256Colors() ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";
    std::cout << Terminal::reset();
}

/**
 * Display about information
 */
void showAbout() {
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << Terminal::bold();

    if (Terminal::supportsUTF8()) {
        std::cout << Terminal::ICON_INFO << " ";
    }

    std::cout << "About " << APP_NAME << "\n" << Terminal::reset();
    std::cout << "──────────────────────────────────────────────────\n\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Version:     " << Terminal::fg(Terminal::Color::BRIGHT_WHITE) << VERSION << "\n";
    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Author:      " << Terminal::fg(Terminal::Color::BRIGHT_WHITE) << AUTHOR << "\n";
    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Description: " << Terminal::fg(Terminal::Color::BRIGHT_WHITE) << DESCRIPTION << "\n\n";
    std::cout << Terminal::reset();

    std::cout << Terminal::fg(Terminal::Color::BRIGHT_YELLOW);
    std::cout << "Purpose:\n" << Terminal::reset();
    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "This tool allows security researchers and penetration testers to analyze\n";
    std::cout << "and test Telegram bot tokens discovered during security assessments. It\n";
    std::cout << "provides capabilities for exploring the Telegram Bot API, analyzing bot\n";
    std::cout << "permissions, and testing security configurations.\n\n";
    std::cout << Terminal::reset();

    std::cout << Terminal::fg(Terminal::Color::BRIGHT_YELLOW);
    std::cout << "Features:\n" << Terminal::reset();
    std::cout << Terminal::fg(Terminal::Color::GREEN);
    if (Terminal::supportsUTF8()) {
        std::cout << "  " << Terminal::ICON_SUCCESS << " API exploration and testing\n";
        std::cout << "  " << Terminal::ICON_SUCCESS << " Bot token validation\n";
        std::cout << "  " << Terminal::ICON_SUCCESS << " Permission analysis\n";
        std::cout << "  " << Terminal::ICON_SUCCESS << " Security assessment tools\n";
    } else {
        std::cout << "  • API exploration and testing\n";
        std::cout << "  • Bot token validation\n";
        std::cout << "  • Permission analysis\n";
        std::cout << "  • Security assessment tools\n";
    }
    std::cout << Terminal::reset() << "\n";

    std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK);
    std::cout << "Built with C++ for performance and security on Linux platforms.\n";
    std::cout << Terminal::reset();
}

/**
 * Get bot token from command line or environment
 */
std::string getToken(int argc, char* argv[]) {
    // First check command line arguments
    for (int i = 1; i < argc - 1; i++) {
        std::string arg = argv[i];
        if (arg == "--token") {
            return argv[i + 1];
        }
    }

    // Then check environment variable
    const char* envToken = std::getenv("TGDIGGER_TOKEN");
    if (envToken) {
        return std::string(envToken);
    }

    // Check config file
    Config& config = Config::getInstance();
    std::string configToken = config.get("bot_token", "");
    if (!configToken.empty()) {
        return configToken;
    }

    return "";
}

/**
 * Validate bot token
 */
void validateToken(const std::string& token) {
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << Terminal::bold();
    if (Terminal::supportsUTF8()) {
        std::cout << Terminal::ICON_SEARCH << " ";
    }
    std::cout << "Validating Bot Token" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n\n";

    // Check token format first
    if (!TelegramApi::isValidTokenFormat(token)) {
        Terminal::error("Invalid token format");
        std::cout << "\n" << Terminal::fg(Terminal::Color::YELLOW);
        std::cout << "Token should be in format: " << Terminal::fg(Terminal::Color::BRIGHT_WHITE);
        std::cout << "<bot_id>:<token_hash>\n";
        std::cout << Terminal::fg(Terminal::Color::YELLOW);
        std::cout << "Example: " << Terminal::fg(Terminal::Color::BRIGHT_WHITE);
        std::cout << "123456789:ABCdefGHIjklMNOpqrsTUVwxyz-1234567890\n";
        std::cout << Terminal::reset();
        return;
    }

    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Token format: ";
    Terminal::success("Valid");
    std::cout << "\n";

    // Create API client
    std::cout << Terminal::fg(Terminal::Color::CYAN);
    std::cout << "Connecting to Telegram API...\n";
    std::cout << Terminal::reset();

    TelegramApi api(token);

    // Validate token via API
    std::cout << Terminal::fg(Terminal::Color::CYAN);
    std::cout << "Calling getMe endpoint...\n";
    std::cout << Terminal::reset();

    BotInfo botInfo;
    bool valid = api.getMe(botInfo);

    std::cout << "\n";

    if (!valid) {
        Terminal::error("Token validation failed");
        std::cout << "\n" << Terminal::fg(Terminal::Color::RED);
        std::cout << "Error: " << Terminal::fg(Terminal::Color::BRIGHT_RED);
        std::cout << api.getLastError() << "\n";
        std::cout << Terminal::reset();
        return;
    }

    // Display bot information
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << Terminal::bold();
    if (Terminal::supportsUTF8()) {
        std::cout << Terminal::ICON_SUCCESS << " ";
    }
    std::cout << "Token is valid!" << Terminal::reset() << "\n\n";

    std::cout << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << Terminal::bold();
    std::cout << "Bot Information:" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Bot ID:           " << Terminal::fg(Terminal::Color::BRIGHT_WHITE);
    std::cout << botInfo.id << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Bot Name:         " << Terminal::fg(Terminal::Color::BRIGHT_WHITE);
    std::cout << botInfo.firstName << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Username:         " << Terminal::fg(Terminal::Color::BRIGHT_CYAN);
    std::cout << "@" << botInfo.username << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Is Bot:           " << (botInfo.isBot ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::reset() << "\n";

    std::cout << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << Terminal::bold();
    std::cout << "Capabilities:" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Can Join Groups:         " << (botInfo.canJoinGroups ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Can Read All Messages:   " << (botInfo.canReadAllGroupMessages ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Supports Inline Queries: " << (botInfo.supportsInlineQueries ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::reset() << "\n";

    // Save token information
    Config& config = Config::getInstance();

    // Create valid-tokens directory
    std::string validTokensDir = config.getValidTokensDir();
    struct stat st;
    if (stat(validTokensDir.c_str(), &st) == -1) {
        mkdir(validTokensDir.c_str(), 0700);
    }

    // Save bot info to file
    std::string tokenFile = validTokensDir + "/" + token;
    std::ofstream outFile(tokenFile);
    if (outFile.is_open()) {
        outFile << "Bot ID: " << botInfo.id << "\n";
        outFile << "Bot Name: " << botInfo.firstName << "\n";
        outFile << "Username: @" << botInfo.username << "\n";
        outFile << "Is Bot: " << (botInfo.isBot ? "Yes" : "No") << "\n";
        outFile << "Can Join Groups: " << (botInfo.canJoinGroups ? "Yes" : "No") << "\n";
        outFile << "Can Read All Messages: " << (botInfo.canReadAllGroupMessages ? "Yes" : "No") << "\n";
        outFile << "Supports Inline Queries: " << (botInfo.supportsInlineQueries ? "Yes" : "No") << "\n";

        // Add timestamp
        time_t now = time(nullptr);
        struct tm* timeinfo = localtime(&now);
        char timeStr[64];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
        outFile << "\nValidated: " << timeStr << "\n";

        outFile.close();

        // Set secure permissions
        chmod(tokenFile.c_str(), 0600);

        std::cout << Terminal::fg(Terminal::Color::BRIGHT_GREEN);
        if (Terminal::supportsUTF8()) {
            std::cout << Terminal::ICON_CONFIG << " ";
        }
        std::cout << "Token info saved to: " << Terminal::fg(Terminal::Color::CYAN);
        std::cout << tokenFile << "\n" << Terminal::reset();
    }

    // Save to tokens-seen file
    if (!config.isTokenSeen(token)) {
        if (config.saveTokenSeen(token)) {
            std::cout << Terminal::fg(Terminal::Color::BRIGHT_GREEN);
            if (Terminal::supportsUTF8()) {
                std::cout << Terminal::ICON_SUCCESS << " ";
            }
            std::cout << "Token added to seen list\n" << Terminal::reset();
        }
    } else {
        std::cout << Terminal::fg(Terminal::Color::YELLOW);
        if (Terminal::supportsUTF8()) {
            std::cout << Terminal::ICON_INFO << " ";
        }
        std::cout << "Token was previously seen\n" << Terminal::reset();
    }
}

/**
 * Main entry point
 */
int main(int argc, char* argv[]) {
    // Initialize configuration
    Config& config = Config::getInstance();
    config.load();

    // Parse command line arguments
    if (argc < 2) {
        showHelp();
        return 0;
    }

    // Check for --validate flag
    bool shouldValidate = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--validate") {
            shouldValidate = true;
            break;
        }
    }

    // Handle validation
    if (shouldValidate) {
        std::string token = getToken(argc, argv);

        if (token.empty()) {
            Terminal::error("No bot token provided");
            std::cout << "\n" << Terminal::fg(Terminal::Color::YELLOW);
            std::cout << "Please provide a token using one of these methods:\n";
            std::cout << "  1. Command line: " << Terminal::fg(Terminal::Color::BRIGHT_GREEN);
            std::cout << "--token <TOKEN>\n";
            std::cout << Terminal::fg(Terminal::Color::YELLOW);
            std::cout << "  2. Environment:  " << Terminal::fg(Terminal::Color::BRIGHT_MAGENTA);
            std::cout << "export TGDIGGER_TOKEN=<TOKEN>\n";
            std::cout << Terminal::fg(Terminal::Color::YELLOW);
            std::cout << "  3. Config file:  " << Terminal::fg(Terminal::Color::BRIGHT_CYAN);
            std::cout << "~/.telegramdigger/settings.conf\n";
            std::cout << Terminal::reset();
            return 1;
        }

        validateToken(token);
        return 0;
    }

    // Handle other commands
    std::string arg = argv[1];

    if (arg == "--help" || arg == "-h") {
        showHelp();
        return 0;
    } else if (arg == "--version" || arg == "-v") {
        showVersion();
        return 0;
    } else if (arg == "--about") {
        showAbout();
        return 0;
    } else if (arg == "--token") {
        // Token flag needs to be used with another command
        if (argc < 3) {
            Terminal::error("--token requires a value");
            std::cout << "\nUse " << Terminal::colorize("--help", Terminal::Color::BRIGHT_GREEN)
                      << " to see available options.\n";
            return 1;
        }
        Terminal::error("--token must be used with another command (e.g., --validate)");
        std::cout << "\nUse " << Terminal::colorize("--help", Terminal::Color::BRIGHT_GREEN)
                  << " to see available options.\n";
        return 1;
    } else {
        Terminal::error("Unknown option: " + arg);
        std::cout << "\nUse " << Terminal::colorize("--help", Terminal::Color::BRIGHT_GREEN)
                  << " to see available options.\n";
        return 1;
    }

    return 0;
}
