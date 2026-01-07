#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include <memory>

namespace TelegramDigger {

/**
 * Configuration manager for TelegramDigger
 * Handles reading/writing to $HOME/.telegramdigger/settings.conf
 */
class Config {
public:
    /**
     * Get singleton instance
     */
    static Config& getInstance();

    /**
     * Load configuration from file
     */
    bool load();

    /**
     * Save configuration to file
     */
    bool save();

    /**
     * Get configuration value
     */
    std::string get(const std::string& key, const std::string& defaultValue = "") const;

    /**
     * Set configuration value
     */
    void set(const std::string& key, const std::string& value);

    /**
     * Get configuration directory path
     */
    std::string getConfigDir() const;

    /**
     * Get configuration file path
     */
    std::string getConfigFile() const;

    /**
     * Ensure configuration directory exists
     */
    bool ensureConfigDir();

    /**
     * Get valid tokens directory path
     */
    std::string getValidTokensDir() const;

    /**
     * Get tokens seen file path
     */
    std::string getTokensSeenFile() const;

    /**
     * Save token to tokens-seen file
     */
    bool saveTokenSeen(const std::string& token, const std::string& note = "");

    /**
     * Check if token has been seen before
     */
    bool isTokenSeen(const std::string& token) const;

private:
    Config();
    ~Config() = default;

    // Prevent copying
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    std::map<std::string, std::string> settings_;
    std::string configDir_;
    std::string configFile_;

    /**
     * Parse configuration line
     */
    bool parseLine(const std::string& line, std::string& key, std::string& value);
};

} // namespace TelegramDigger

#endif // CONFIG_H
