#ifndef TELEGRAM_API_H
#define TELEGRAM_API_H

#include "http_client.h"
#include <string>
#include <memory>

namespace TelegramDigger {

/**
 * Bot information structure
 */
struct BotInfo {
    long long id;
    bool isBot;
    std::string firstName;
    std::string username;
    bool canJoinGroups;
    bool canReadAllGroupMessages;
    bool supportsInlineQueries;

    BotInfo() : id(0), isBot(false), canJoinGroups(false),
                canReadAllGroupMessages(false), supportsInlineQueries(false) {}
};

/**
 * API response structure
 */
struct ApiResponse {
    bool ok;
    std::string description;
    int errorCode;

    ApiResponse() : ok(false), errorCode(0) {}
};

/**
 * Telegram Bot API client
 */
class TelegramApi {
public:
    /**
     * Constructor with bot token
     */
    explicit TelegramApi(const std::string& token);

    /**
     * Destructor
     */
    ~TelegramApi();

    /**
     * Validate bot token by calling getMe endpoint
     * Returns true if token is valid and bot is active
     */
    bool validateToken();

    /**
     * Get bot information
     */
    bool getMe(BotInfo& botInfo);

    /**
     * Get last API response
     */
    const ApiResponse& getLastResponse() const;

    /**
     * Get last error message
     */
    std::string getLastError() const;

    /**
     * Check if token format is valid (basic validation)
     */
    static bool isValidTokenFormat(const std::string& token);

private:
    std::string token_;
    std::string apiBaseUrl_;
    std::unique_ptr<HttpClient> httpClient_;
    ApiResponse lastResponse_;

    /**
     * Build API URL for endpoint
     */
    std::string buildUrl(const std::string& endpoint) const;

    /**
     * Parse JSON response (simple parser for our needs)
     */
    bool parseJsonResponse(const std::string& json, ApiResponse& response);

    /**
     * Extract string value from JSON
     */
    std::string extractJsonString(const std::string& json, const std::string& key);

    /**
     * Extract boolean value from JSON
     */
    bool extractJsonBool(const std::string& json, const std::string& key);

    /**
     * Extract integer value from JSON
     */
    long long extractJsonInt(const std::string& json, const std::string& key);
};

} // namespace TelegramDigger

#endif // TELEGRAM_API_H
