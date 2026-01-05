#ifndef TELEGRAM_API_H
#define TELEGRAM_API_H

#include "http_client.h"
#include <string>
#include <memory>
#include <vector>

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
 * Bot administrator rights structure
 */
struct BotAdminRights {
    bool isAnonymous;
    bool canManageChat;
    bool canDeleteMessages;
    bool canManageVideoChats;
    bool canRestrictMembers;
    bool canPromoteMembers;
    bool canChangeInfo;
    bool canInviteUsers;
    bool canPostMessages;       // Channels only
    bool canEditMessages;       // Channels only
    bool canPinMessages;
    bool canManageTopics;

    BotAdminRights() : isAnonymous(false), canManageChat(false),
                       canDeleteMessages(false), canManageVideoChats(false),
                       canRestrictMembers(false), canPromoteMembers(false),
                       canChangeInfo(false), canInviteUsers(false),
                       canPostMessages(false), canEditMessages(false),
                       canPinMessages(false), canManageTopics(false) {}
};

/**
 * Webhook information structure
 */
struct WebhookInfo {
    std::string url;
    bool hasCustomCertificate;
    int pendingUpdateCount;
    std::string ipAddress;
    long long lastErrorDate;
    std::string lastErrorMessage;
    int maxConnections;
    std::vector<std::string> allowedUpdates;

    WebhookInfo() : hasCustomCertificate(false), pendingUpdateCount(0),
                    lastErrorDate(0), maxConnections(0) {}
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
     * Get default bot administrator rights
     */
    bool getMyDefaultAdministratorRights(BotAdminRights& groupRights, BotAdminRights& channelRights);

    /**
     * Get webhook information
     */
    bool getWebhookInfo(WebhookInfo& webhookInfo);

    /**
     * Set webhook URL
     */
    bool setWebhook(const std::string& url, int maxConnections = 40);

    /**
     * Delete webhook
     */
    bool deleteWebhook();

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
