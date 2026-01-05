#include "terminal.h"
#include "config.h"
#include "version.h"
#include "telegram_api.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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

    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << "--bulk-validate"
              << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << " [FILE]"
              << Terminal::fg(Terminal::Color::WHITE) << "  Bulk validate tokens (default: tokens-seen)\n";

    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << "--read-botrights"
              << Terminal::fg(Terminal::Color::WHITE) << "      Read default bot administrator rights\n";

    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << "--webhook-get"
              << Terminal::fg(Terminal::Color::WHITE) << "        Get current webhook information\n";

    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << "--webhook-set"
              << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << " <URL>"
              << Terminal::fg(Terminal::Color::WHITE) << "    Set webhook URL\n";

    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << "--webhook-delete"
              << Terminal::fg(Terminal::Color::WHITE) << "     Delete webhook\n";

    std::cout << "  " << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << "--webhook-info"
              << Terminal::fg(Terminal::Color::WHITE) << "       Get webhook status (alias for --webhook-get)\n";

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
 * Bulk validate tokens from file
 */
void bulkValidateTokens(const std::string& filePath) {
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << Terminal::bold();
    if (Terminal::supportsUTF8()) {
        std::cout << Terminal::ICON_SEARCH << " ";
    }
    std::cout << "Bulk Token Validation" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n\n";

    // Read tokens from file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        Terminal::error("Cannot open file: " + filePath);
        return;
    }

    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Reading tokens from: " << Terminal::fg(Terminal::Color::CYAN);
    std::cout << filePath << "\n" << Terminal::reset() << "\n";

    std::vector<std::string> tokens;
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Extract token (handle CSV format for tokens-seen)
        size_t pos = line.find('#');
        std::string token = (pos != std::string::npos) ? line.substr(0, pos) : line;

        // Trim whitespace
        size_t start = token.find_first_not_of(" \t\r\n");
        size_t end = token.find_last_not_of(" \t\r\n");
        if (start != std::string::npos && end != std::string::npos) {
            token = token.substr(start, end - start + 1);
        }

        if (!token.empty() && TelegramApi::isValidTokenFormat(token)) {
            tokens.push_back(token);
        }
    }
    file.close();

    if (tokens.empty()) {
        Terminal::warning("No valid tokens found in file");
        return;
    }

    std::cout << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Found " << Terminal::fg(Terminal::Color::BRIGHT_YELLOW);
    std::cout << tokens.size() << Terminal::fg(Terminal::Color::WHITE);
    std::cout << " token(s) to validate\n" << Terminal::reset() << "\n";

    // Validate each token
    int validCount = 0;
    int invalidCount = 0;

    for (size_t i = 0; i < tokens.size(); i++) {
        const std::string& token = tokens[i];

        // Create API client and validate
        TelegramApi api(token);
        BotInfo botInfo;
        bool valid = api.getMe(botInfo);

        // Print result on one line
        std::cout << "[" << (i + 1) << "/" << tokens.size() << "] ";

        // Show abbreviated token
        std::string shortToken = token.substr(0, 15) + "...";
        std::cout << Terminal::fg(Terminal::Color::BRIGHT_BLACK) << shortToken << Terminal::reset() << " - ";

        if (valid) {
            validCount++;
            std::cout << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << Terminal::bold();
            if (Terminal::supportsUTF8()) {
                std::cout << Terminal::ICON_SUCCESS << " ";
            }
            std::cout << "VALID" << Terminal::reset();
            std::cout << Terminal::fg(Terminal::Color::WHITE) << " - ";
            std::cout << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << botInfo.firstName;
            std::cout << Terminal::fg(Terminal::Color::CYAN) << " (@" << botInfo.username << ")";
            std::cout << Terminal::reset() << "\n";
        } else {
            invalidCount++;
            std::cout << Terminal::fg(Terminal::Color::BRIGHT_RED) << Terminal::bold();
            if (Terminal::supportsUTF8()) {
                std::cout << Terminal::ICON_ERROR << " ";
            }
            std::cout << "INVALID" << Terminal::reset();
            std::cout << Terminal::fg(Terminal::Color::RED) << " - " << api.getLastError();
            std::cout << Terminal::reset() << "\n";
        }
    }

    // Summary
    std::cout << "\n" << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << Terminal::bold();
    std::cout << "Summary:" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Total tokens:    ";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_WHITE) << tokens.size() << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Valid tokens:    ";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_GREEN) << validCount << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Invalid tokens:  ";
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_RED) << invalidCount << "\n";

    std::cout << Terminal::reset();
}

/**
 * Read and display bot administrator rights
 */
void readBotRights(const std::string& token) {
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << Terminal::bold();
    if (Terminal::supportsUTF8()) {
        std::cout << Terminal::ICON_KEY << " ";
    }
    std::cout << "Bot Administrator Rights" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n\n";

    // Create API client
    TelegramApi api(token);
    BotAdminRights groupRights, channelRights;

    std::cout << Terminal::fg(Terminal::Color::CYAN);
    std::cout << "Fetching default administrator rights...\n";
    std::cout << Terminal::reset();

    bool success = api.getMyDefaultAdministratorRights(groupRights, channelRights);

    std::cout << "\n";

    if (!success) {
        Terminal::error("Failed to retrieve administrator rights");
        std::cout << "\n" << Terminal::fg(Terminal::Color::RED);
        std::cout << "Error: " << Terminal::fg(Terminal::Color::BRIGHT_RED);
        std::cout << api.getLastError() << "\n";
        std::cout << Terminal::reset();
        return;
    }

    // Display group/supergroup rights
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << Terminal::bold();
    std::cout << "Groups & Supergroups:" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Anonymous Admin:        " << (groupRights.isAnonymous ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Manage Chat:            " << (groupRights.canManageChat ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Delete Messages:        " << (groupRights.canDeleteMessages ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Manage Video Chats:     " << (groupRights.canManageVideoChats ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Restrict Members:       " << (groupRights.canRestrictMembers ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Promote Members:        " << (groupRights.canPromoteMembers ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Change Info:            " << (groupRights.canChangeInfo ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Invite Users:           " << (groupRights.canInviteUsers ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Pin Messages:           " << (groupRights.canPinMessages ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Manage Topics:          " << (groupRights.canManageTopics ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    // Display channel rights
    std::cout << "\n" << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << Terminal::bold();
    std::cout << "Channels:" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Anonymous Admin:        " << (channelRights.isAnonymous ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Manage Chat:            " << (channelRights.canManageChat ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Delete Messages:        " << (channelRights.canDeleteMessages ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Manage Video Chats:     " << (channelRights.canManageVideoChats ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Restrict Members:       " << (channelRights.canRestrictMembers ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Promote Members:        " << (channelRights.canPromoteMembers ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Change Info:            " << (channelRights.canChangeInfo ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Invite Users:           " << (channelRights.canInviteUsers ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Post Messages:          " << (channelRights.canPostMessages ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Edit Messages:          " << (channelRights.canEditMessages ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Pin Messages:           " << (channelRights.canPinMessages ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::fg(Terminal::Color::WHITE) << "Manage Topics:          " << (channelRights.canManageTopics ?
        Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
        Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

    std::cout << Terminal::reset();
}

/**
 * Get webhook information
 */
void getWebhookInfo(const std::string& token) {
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << Terminal::bold();
    std::cout << " Webhook Information" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n\n";

    std::cout << Terminal::fg(Terminal::Color::CYAN) << "Fetching webhook information...\n" << Terminal::reset();

    TelegramApi api(token);
    WebhookInfo webhookInfo;
    bool success = api.getWebhookInfo(webhookInfo);

    if (!success) {
        std::cout << "\n";
        Terminal::error("Failed to retrieve webhook information");
        std::cout << "\n" << Terminal::fg(Terminal::Color::RED) << "Error: " << Terminal::fg(Terminal::Color::BRIGHT_RED);
        std::cout << api.getLastError() << Terminal::reset() << "\n";
        return;
    }

    std::cout << "\n";
    Terminal::success("Successfully retrieved webhook information");

    std::cout << "\n" << Terminal::fg(Terminal::Color::BRIGHT_YELLOW) << Terminal::bold();
    std::cout << "Webhook Status:" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n";

    if (webhookInfo.url.empty()) {
        std::cout << Terminal::fg(Terminal::Color::YELLOW) << "No webhook is currently set\n";
        std::cout << Terminal::fg(Terminal::Color::WHITE) << "Use " << Terminal::fg(Terminal::Color::BRIGHT_GREEN);
        std::cout << "--webhook-set <URL>" << Terminal::fg(Terminal::Color::WHITE);
        std::cout << " to configure a webhook\n" << Terminal::reset();
    } else {
        std::cout << Terminal::fg(Terminal::Color::WHITE) << "URL:                    " << Terminal::fg(Terminal::Color::BRIGHT_CYAN);
        std::cout << webhookInfo.url << "\n" << Terminal::reset();

        std::cout << Terminal::fg(Terminal::Color::WHITE) << "Custom Certificate:     " << (webhookInfo.hasCustomCertificate ?
            Terminal::colorize("Yes", Terminal::Color::BRIGHT_GREEN) :
            Terminal::colorize("No", Terminal::Color::BRIGHT_RED)) << "\n";

        std::cout << Terminal::fg(Terminal::Color::WHITE) << "Pending Updates:        " << Terminal::fg(Terminal::Color::BRIGHT_WHITE);
        std::cout << webhookInfo.pendingUpdateCount << "\n" << Terminal::reset();

        if (!webhookInfo.ipAddress.empty()) {
            std::cout << Terminal::fg(Terminal::Color::WHITE) << "IP Address:             " << Terminal::fg(Terminal::Color::BRIGHT_WHITE);
            std::cout << webhookInfo.ipAddress << "\n" << Terminal::reset();
        }

        if (webhookInfo.maxConnections > 0) {
            std::cout << Terminal::fg(Terminal::Color::WHITE) << "Max Connections:        " << Terminal::fg(Terminal::Color::BRIGHT_WHITE);
            std::cout << webhookInfo.maxConnections << "\n" << Terminal::reset();
        }

        if (webhookInfo.lastErrorDate > 0) {
            std::cout << "\n" << Terminal::fg(Terminal::Color::BRIGHT_RED) << Terminal::bold();
            std::cout << "Last Error:" << Terminal::reset() << "\n";
            std::cout << Terminal::fg(Terminal::Color::WHITE) << "Date:                   " << Terminal::fg(Terminal::Color::BRIGHT_WHITE);
            time_t errorTime = static_cast<time_t>(webhookInfo.lastErrorDate);
            struct tm* timeinfo = localtime(&errorTime);
            char dateStr[20];
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", timeinfo);
            std::cout << dateStr << "\n" << Terminal::reset();

            if (!webhookInfo.lastErrorMessage.empty()) {
                std::cout << Terminal::fg(Terminal::Color::WHITE) << "Message:                " << Terminal::fg(Terminal::Color::BRIGHT_RED);
                std::cout << webhookInfo.lastErrorMessage << "\n" << Terminal::reset();
            }
        }
    }

    std::cout << Terminal::reset();
}

/**
 * Set webhook URL
 */
void setWebhook(const std::string& token, const std::string& url) {
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << Terminal::bold();
    std::cout << " Set Webhook" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n\n";

    std::cout << Terminal::fg(Terminal::Color::CYAN) << "Setting webhook URL...\n" << Terminal::reset();
    std::cout << Terminal::fg(Terminal::Color::WHITE) << "URL: " << Terminal::fg(Terminal::Color::BRIGHT_CYAN);
    std::cout << url << "\n\n" << Terminal::reset();

    TelegramApi api(token);
    bool success = api.setWebhook(url);

    if (!success) {
        Terminal::error("Failed to set webhook");
        std::cout << "\n" << Terminal::fg(Terminal::Color::RED) << "Error: " << Terminal::fg(Terminal::Color::BRIGHT_RED);
        std::cout << api.getLastError() << Terminal::reset() << "\n";
        return;
    }

    Terminal::success("Webhook URL has been set successfully");

    std::cout << "\n" << Terminal::fg(Terminal::Color::YELLOW);
    std::cout << "Note: Make sure your webhook URL:\n";
    std::cout << "  - Uses HTTPS (required by Telegram)\n";
    std::cout << "  - Has a valid SSL certificate\n";
    std::cout << "  - Is publicly accessible\n";
    std::cout << "  - Responds with HTTP 200 OK to POST requests\n";
    std::cout << Terminal::reset();
}

/**
 * Delete webhook
 */
void deleteWebhook(const std::string& token) {
    std::cout << Terminal::fg(Terminal::Color::BRIGHT_CYAN) << Terminal::bold();
    std::cout << " Delete Webhook" << Terminal::reset() << "\n";
    std::cout << "──────────────────────────────────────────────────\n\n";

    std::cout << Terminal::fg(Terminal::Color::CYAN) << "Deleting webhook...\n" << Terminal::reset();

    TelegramApi api(token);
    bool success = api.deleteWebhook();

    if (!success) {
        std::cout << "\n";
        Terminal::error("Failed to delete webhook");
        std::cout << "\n" << Terminal::fg(Terminal::Color::RED) << "Error: " << Terminal::fg(Terminal::Color::BRIGHT_RED);
        std::cout << api.getLastError() << Terminal::reset() << "\n";
        return;
    }

    std::cout << "\n";
    Terminal::success("Webhook has been deleted successfully");

    std::cout << "\n" << Terminal::fg(Terminal::Color::WHITE);
    std::cout << "Your bot will no longer receive updates via webhook.\n";
    std::cout << "You can now use long polling (getUpdates) instead.\n";
    std::cout << Terminal::reset();
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

    // Check for --bulk-validate flag
    bool shouldBulkValidate = false;
    std::string bulkFile = "";
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--bulk-validate") {
            shouldBulkValidate = true;
            // Check if file path is provided
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                bulkFile = argv[i + 1];
            }
            break;
        }
    }

    // Handle bulk validation
    if (shouldBulkValidate) {
        // Use tokens-seen file if no file specified
        if (bulkFile.empty()) {
            bulkFile = config.getTokensSeenFile();
        }

        bulkValidateTokens(bulkFile);
        return 0;
    }

    // Check for --read-botrights flag
    bool shouldReadRights = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--read-botrights") {
            shouldReadRights = true;
            break;
        }
    }

    // Handle read bot rights
    if (shouldReadRights) {
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

        readBotRights(token);
        return 0;
    }

    // Check for --webhook-get or --webhook-info flag
    bool shouldGetWebhook = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--webhook-get" || arg == "--webhook-info") {
            shouldGetWebhook = true;
            break;
        }
    }

    // Handle get webhook info
    if (shouldGetWebhook) {
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

        getWebhookInfo(token);
        return 0;
    }

    // Check for --webhook-set flag
    bool shouldSetWebhook = false;
    std::string webhookUrl = "";
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--webhook-set") {
            shouldSetWebhook = true;
            // Check if URL is provided
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                webhookUrl = argv[i + 1];
            }
            break;
        }
    }

    // Handle set webhook
    if (shouldSetWebhook) {
        if (webhookUrl.empty()) {
            Terminal::error("No webhook URL provided");
            std::cout << "\n" << Terminal::fg(Terminal::Color::YELLOW);
            std::cout << "Usage: " << Terminal::fg(Terminal::Color::BRIGHT_GREEN);
            std::cout << "--webhook-set <URL>\n";
            std::cout << Terminal::fg(Terminal::Color::YELLOW);
            std::cout << "Example: " << Terminal::fg(Terminal::Color::BRIGHT_GREEN);
            std::cout << "--webhook-set https://example.com/webhook\n";
            std::cout << Terminal::reset();
            return 1;
        }

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

        setWebhook(token, webhookUrl);
        return 0;
    }

    // Check for --webhook-delete flag
    bool shouldDeleteWebhook = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--webhook-delete") {
            shouldDeleteWebhook = true;
            break;
        }
    }

    // Handle delete webhook
    if (shouldDeleteWebhook) {
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

        deleteWebhook(token);
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
