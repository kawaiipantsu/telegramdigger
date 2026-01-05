#include "config.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace TelegramDigger {

Config::Config() {
    const char* home = std::getenv("HOME");
    if (home) {
        configDir_ = std::string(home) + "/.telegramdigger";
        configFile_ = configDir_ + "/settings.conf";
    } else {
        configDir_ = "./.telegramdigger";
        configFile_ = configDir_ + "/settings.conf";
    }
}

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

bool Config::ensureConfigDir() {
    struct stat st;
    if (stat(configDir_.c_str(), &st) == -1) {
        if (mkdir(configDir_.c_str(), 0700) == -1) {
            return false;
        }
    }
    return true;
}

bool Config::load() {
    std::ifstream file(configFile_);
    if (!file.is_open()) {
        // Config file doesn't exist yet, that's okay
        return true;
    }

    settings_.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::string key, value;
        if (parseLine(line, key, value)) {
            settings_[key] = value;
        }
    }

    file.close();
    return true;
}

bool Config::save() {
    if (!ensureConfigDir()) {
        return false;
    }

    std::ofstream file(configFile_);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& pair : settings_) {
        file << pair.first << "=" << pair.second << std::endl;
    }

    file.close();

    // Set secure permissions (0600 - owner read/write only)
    chmod(configFile_.c_str(), 0600);

    return true;
}

std::string Config::get(const std::string& key, const std::string& defaultValue) const {
    auto it = settings_.find(key);
    if (it != settings_.end()) {
        return it->second;
    }
    return defaultValue;
}

void Config::set(const std::string& key, const std::string& value) {
    settings_[key] = value;
}

std::string Config::getConfigDir() const {
    return configDir_;
}

std::string Config::getConfigFile() const {
    return configFile_;
}

bool Config::parseLine(const std::string& line, std::string& key, std::string& value) {
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#' || line[0] == ';') {
        return false;
    }

    size_t pos = line.find('=');
    if (pos == std::string::npos) {
        return false;
    }

    key = line.substr(0, pos);
    value = line.substr(pos + 1);

    // Trim whitespace
    size_t start = key.find_first_not_of(" \t");
    size_t end = key.find_last_not_of(" \t");
    if (start != std::string::npos && end != std::string::npos) {
        key = key.substr(start, end - start + 1);
    }

    start = value.find_first_not_of(" \t");
    end = value.find_last_not_of(" \t");
    if (start != std::string::npos && end != std::string::npos) {
        value = value.substr(start, end - start + 1);
    }

    return !key.empty();
}

std::string Config::getValidTokensDir() const {
    return configDir_ + "/valid-tokens";
}

std::string Config::getTokensSeenFile() const {
    return configDir_ + "/tokens-seen";
}

bool Config::saveTokenSeen(const std::string& token) {
    if (!ensureConfigDir()) {
        return false;
    }

    // Get current date/time
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", timeinfo);

    // Append to tokens-seen file
    std::ofstream file(getTokensSeenFile(), std::ios::app);
    if (!file.is_open()) {
        return false;
    }

    file << token << "#" << dateStr << std::endl;
    file.close();

    return true;
}

bool Config::isTokenSeen(const std::string& token) const {
    std::ifstream file(getTokensSeenFile());
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('#');
        if (pos != std::string::npos) {
            std::string seenToken = line.substr(0, pos);
            if (seenToken == token) {
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}

} // namespace TelegramDigger
