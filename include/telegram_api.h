#ifndef TELEGRAM_API_H
#define TELEGRAM_API_H

#include "http_client.h"
#include <string>
#include <memory>
#include <vector>
#include <set>

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
 * Bot command structure
 */
struct BotCommand {
    std::string command;
    std::string description;

    BotCommand() {}
    BotCommand(const std::string& cmd, const std::string& desc)
        : command(cmd), description(desc) {}
};

/**
 * Update structure (simplified for analysis)
 */
struct Update {
    long long updateId;
    std::string type;          // "message", "edited_message", "channel_post", etc.
    long long chatId;
    std::string chatType;      // "private", "group", "supergroup", "channel"
    std::string chatTitle;
    long long userId;
    std::string username;
    std::string firstName;
    std::string text;
    long long timestamp;

    Update() : updateId(0), chatId(0), userId(0), timestamp(0) {}
};

/**
 * Chat administrator structure
 */
struct ChatAdmin {
    long long userId;
    std::string username;
    std::string firstName;
    std::string status;        // "creator", "administrator"
    bool isAnonymous;
    std::string customTitle;

    ChatAdmin() : userId(0), isAnonymous(false) {}
};

/**
 * Chat information structure
 */
struct ChatInfo {
    long long chatId;
    std::string type;
    std::string title;
    std::string username;
    int memberCount;
    std::string description;
    bool hasProtectedContent;
    std::vector<ChatAdmin> admins;

    ChatInfo() : chatId(0), memberCount(0), hasProtectedContent(false) {}
};

/**
 * Security finding structure
 */
struct SecurityFinding {
    std::string category;      // "CRITICAL", "HIGH", "MEDIUM", "LOW", "INFO"
    std::string title;
    std::string description;
    std::string recommendation;

    SecurityFinding() {}
    SecurityFinding(const std::string& cat, const std::string& t,
                   const std::string& desc, const std::string& rec)
        : category(cat), title(t), description(desc), recommendation(rec) {}
};

/**
 * Complete bot analysis data container
 */
struct BotAnalysis {
    // Basic bot info
    BotInfo botInfo;
    BotAdminRights groupRights;
    BotAdminRights channelRights;
    WebhookInfo webhookInfo;

    // Commands and descriptions
    std::vector<BotCommand> commands;
    std::string description;
    std::string shortDescription;
    std::string displayName;

    // Updates analysis
    int totalUpdates;
    std::vector<Update> updates;

    // Chat analysis
    std::vector<ChatInfo> chats;

    // User analysis
    std::set<long long> uniqueUsers;

    // Security findings
    std::vector<SecurityFinding> findings;

    BotAnalysis() : totalUpdates(0) {}
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
     * Get bot commands
     */
    bool getMyCommands(std::vector<BotCommand>& commands);

    /**
     * Get bot description
     */
    bool getMyDescription(std::string& description);

    /**
     * Get bot name
     */
    bool getMyName(std::string& name);

    /**
     * Get bot short description
     */
    bool getMyShortDescription(std::string& shortDesc);

    /**
     * Get pending updates
     */
    bool getUpdates(std::vector<Update>& updates, int limit = 100);

    /**
     * Get chat information
     */
    bool getChat(long long chatId, ChatInfo& chatInfo);

    /**
     * Get chat administrators
     */
    bool getChatAdministrators(long long chatId, std::vector<ChatAdmin>& admins);

    /**
     * Get chat member count
     */
    bool getChatMemberCount(long long chatId, int& count);

    /**
     * Send message to a chat
     * @param parseMode Optional: "Markdown", "MarkdownV2", or "HTML"
     * @param disableNotification Optional: Send message silently (no notification)
     * @param disableWebPagePreview Optional: Disable link previews
     */
    bool sendMessage(long long chatId, const std::string& text, long long& messageId,
                    const std::string& parseMode = "", bool disableNotification = false,
                    bool disableWebPagePreview = false);

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
