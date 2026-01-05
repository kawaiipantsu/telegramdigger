#include "telegram_api.h"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace TelegramDigger {

TelegramApi::TelegramApi(const std::string& token)
    : token_(token),
      apiBaseUrl_("https://api.telegram.org/bot"),
      httpClient_(std::make_unique<HttpClient>()) {

    // Set reasonable timeout for API requests
    httpClient_->setTimeout(30);
}

TelegramApi::~TelegramApi() = default;

bool TelegramApi::isValidTokenFormat(const std::string& token) {
    // Telegram bot tokens are in format: <bot_id>:<token_hash>
    // bot_id is numeric, token_hash is alphanumeric
    // Example: 123456789:ABCdefGHIjklMNOpqrsTUVwxyz-1234567890

    if (token.empty()) {
        return false;
    }

    size_t colonPos = token.find(':');
    if (colonPos == std::string::npos) {
        return false;
    }

    // Check bot_id part (should be numeric)
    std::string botId = token.substr(0, colonPos);
    if (botId.empty()) {
        return false;
    }

    for (char c : botId) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    // Check token_hash part (should not be empty)
    std::string tokenHash = token.substr(colonPos + 1);
    if (tokenHash.empty() || tokenHash.length() < 10) {
        return false;
    }

    return true;
}

std::string TelegramApi::buildUrl(const std::string& endpoint) const {
    return apiBaseUrl_ + token_ + "/" + endpoint;
}

std::string TelegramApi::extractJsonString(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);

    if (pos == std::string::npos) {
        return "";
    }

    pos += searchKey.length();

    // Skip whitespace
    while (pos < json.length() && std::isspace(json[pos])) {
        pos++;
    }

    // Check if value is a string (starts with ")
    if (pos >= json.length() || json[pos] != '"') {
        return "";
    }

    pos++; // Skip opening quote
    size_t endPos = pos;

    // Find closing quote (handle escaped quotes)
    while (endPos < json.length()) {
        if (json[endPos] == '"' && (endPos == 0 || json[endPos - 1] != '\\')) {
            break;
        }
        endPos++;
    }

    if (endPos >= json.length()) {
        return "";
    }

    return json.substr(pos, endPos - pos);
}

bool TelegramApi::extractJsonBool(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);

    if (pos == std::string::npos) {
        return false;
    }

    pos += searchKey.length();

    // Skip whitespace
    while (pos < json.length() && std::isspace(json[pos])) {
        pos++;
    }

    // Check for "true"
    if (pos + 4 <= json.length() && json.substr(pos, 4) == "true") {
        return true;
    }

    return false;
}

long long TelegramApi::extractJsonInt(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);

    if (pos == std::string::npos) {
        return 0;
    }

    pos += searchKey.length();

    // Skip whitespace
    while (pos < json.length() && std::isspace(json[pos])) {
        pos++;
    }

    // Extract number
    std::string numStr;
    while (pos < json.length() && (std::isdigit(json[pos]) || json[pos] == '-')) {
        numStr += json[pos];
        pos++;
    }

    if (numStr.empty()) {
        return 0;
    }

    return std::stoll(numStr);
}

bool TelegramApi::parseJsonResponse(const std::string& json, ApiResponse& response) {
    // Extract "ok" field
    response.ok = extractJsonBool(json, "ok");

    // Extract "description" field if present
    response.description = extractJsonString(json, "description");

    // Extract "error_code" field if present
    response.errorCode = static_cast<int>(extractJsonInt(json, "error_code"));

    return true;
}

bool TelegramApi::validateToken() {
    // First check token format
    if (!isValidTokenFormat(token_)) {
        lastResponse_.ok = false;
        lastResponse_.description = "Invalid token format";
        return false;
    }

    // Call getMe endpoint
    std::string url = buildUrl("getMe");
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    // Parse JSON response
    parseJsonResponse(response.body, lastResponse_);

    return lastResponse_.ok;
}

bool TelegramApi::getMe(BotInfo& botInfo) {
    std::string url = buildUrl("getMe");
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    // Parse JSON response
    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    // Extract bot info from result field
    // Find the "result" object
    size_t resultPos = response.body.find("\"result\":");
    if (resultPos == std::string::npos) {
        return false;
    }

    std::string resultJson = response.body.substr(resultPos);

    // Extract bot fields
    botInfo.id = extractJsonInt(resultJson, "id");
    botInfo.isBot = extractJsonBool(resultJson, "is_bot");
    botInfo.firstName = extractJsonString(resultJson, "first_name");
    botInfo.username = extractJsonString(resultJson, "username");
    botInfo.canJoinGroups = extractJsonBool(resultJson, "can_join_groups");
    botInfo.canReadAllGroupMessages = extractJsonBool(resultJson, "can_read_all_group_messages");
    botInfo.supportsInlineQueries = extractJsonBool(resultJson, "supports_inline_queries");

    return true;
}

bool TelegramApi::getMyDefaultAdministratorRights(BotAdminRights& groupRights, BotAdminRights& channelRights) {
    // Get group/supergroup rights
    std::string url = buildUrl("getMyDefaultAdministratorRights");
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    // Parse JSON response
    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    // Extract default administrator rights from result field
    size_t resultPos = response.body.find("\"result\":");
    if (resultPos == std::string::npos) {
        return false;
    }

    std::string resultJson = response.body.substr(resultPos);

    // Extract group/supergroup rights
    groupRights.isAnonymous = extractJsonBool(resultJson, "is_anonymous");
    groupRights.canManageChat = extractJsonBool(resultJson, "can_manage_chat");
    groupRights.canDeleteMessages = extractJsonBool(resultJson, "can_delete_messages");
    groupRights.canManageVideoChats = extractJsonBool(resultJson, "can_manage_video_chats");
    groupRights.canRestrictMembers = extractJsonBool(resultJson, "can_restrict_members");
    groupRights.canPromoteMembers = extractJsonBool(resultJson, "can_promote_members");
    groupRights.canChangeInfo = extractJsonBool(resultJson, "can_change_info");
    groupRights.canInviteUsers = extractJsonBool(resultJson, "can_invite_users");
    groupRights.canPinMessages = extractJsonBool(resultJson, "can_pin_messages");
    groupRights.canManageTopics = extractJsonBool(resultJson, "can_manage_topics");

    // Get channel rights
    url = buildUrl("getMyDefaultAdministratorRights?for_channels=true");
    response = httpClient_->get(url);

    if (!response.success) {
        // If channel rights fail, just use group rights
        channelRights = groupRights;
        return true;
    }

    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        channelRights = groupRights;
        return true;
    }

    // Extract channel rights
    resultPos = response.body.find("\"result\":");
    if (resultPos != std::string::npos) {
        resultJson = response.body.substr(resultPos);

        channelRights.isAnonymous = extractJsonBool(resultJson, "is_anonymous");
        channelRights.canManageChat = extractJsonBool(resultJson, "can_manage_chat");
        channelRights.canDeleteMessages = extractJsonBool(resultJson, "can_delete_messages");
        channelRights.canManageVideoChats = extractJsonBool(resultJson, "can_manage_video_chats");
        channelRights.canRestrictMembers = extractJsonBool(resultJson, "can_restrict_members");
        channelRights.canPromoteMembers = extractJsonBool(resultJson, "can_promote_members");
        channelRights.canChangeInfo = extractJsonBool(resultJson, "can_change_info");
        channelRights.canInviteUsers = extractJsonBool(resultJson, "can_invite_users");
        channelRights.canPostMessages = extractJsonBool(resultJson, "can_post_messages");
        channelRights.canEditMessages = extractJsonBool(resultJson, "can_edit_messages");
        channelRights.canPinMessages = extractJsonBool(resultJson, "can_pin_messages");
        channelRights.canManageTopics = extractJsonBool(resultJson, "can_manage_topics");
    }

    return true;
}

const ApiResponse& TelegramApi::getLastResponse() const {
    return lastResponse_;
}

std::string TelegramApi::getLastError() const {
    if (!lastResponse_.ok && !lastResponse_.description.empty()) {
        return lastResponse_.description;
    }
    return "Unknown error";
}

bool TelegramApi::getWebhookInfo(WebhookInfo& webhookInfo) {
    std::string url = buildUrl("getWebhookInfo");
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    // Parse JSON response
    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    // Extract webhook info from result field
    size_t resultPos = response.body.find("\"result\":");
    if (resultPos == std::string::npos) {
        return false;
    }

    std::string resultJson = response.body.substr(resultPos);

    // Extract webhook fields
    webhookInfo.url = extractJsonString(resultJson, "url");
    webhookInfo.hasCustomCertificate = extractJsonBool(resultJson, "has_custom_certificate");
    webhookInfo.pendingUpdateCount = static_cast<int>(extractJsonInt(resultJson, "pending_update_count"));
    webhookInfo.ipAddress = extractJsonString(resultJson, "ip_address");
    webhookInfo.lastErrorDate = extractJsonInt(resultJson, "last_error_date");
    webhookInfo.lastErrorMessage = extractJsonString(resultJson, "last_error_message");
    webhookInfo.maxConnections = static_cast<int>(extractJsonInt(resultJson, "max_connections"));

    return true;
}

bool TelegramApi::setWebhook(const std::string& webhookUrl, int maxConnections) {
    // Build URL with parameters
    std::ostringstream urlStream;
    urlStream << buildUrl("setWebhook");
    urlStream << "?url=" << webhookUrl;
    urlStream << "&max_connections=" << maxConnections;

    std::string url = urlStream.str();
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    // Parse JSON response
    parseJsonResponse(response.body, lastResponse_);

    return lastResponse_.ok;
}

bool TelegramApi::deleteWebhook() {
    std::string url = buildUrl("deleteWebhook");
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    // Parse JSON response
    parseJsonResponse(response.body, lastResponse_);

    return lastResponse_.ok;
}

} // namespace TelegramDigger
