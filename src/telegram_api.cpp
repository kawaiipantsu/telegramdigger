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

bool TelegramApi::getMyCommands(std::vector<BotCommand>& commands) {
    std::string url = buildUrl("getMyCommands");
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    // Extract commands from result array
    size_t resultPos = response.body.find("\"result\":");
    if (resultPos == std::string::npos) {
        return true; // Empty commands list
    }

    std::string resultJson = response.body.substr(resultPos);

    // Find array start
    size_t arrayStart = resultJson.find('[');
    if (arrayStart == std::string::npos) {
        return true; // Empty array
    }

    // Parse commands manually
    size_t pos = arrayStart + 1;
    while (pos < resultJson.length()) {
        // Find next object
        size_t objStart = resultJson.find('{', pos);
        if (objStart == std::string::npos) break;

        size_t objEnd = resultJson.find('}', objStart);
        if (objEnd == std::string::npos) break;

        std::string objJson = resultJson.substr(objStart, objEnd - objStart + 1);

        BotCommand cmd;
        cmd.command = extractJsonString(objJson, "command");
        cmd.description = extractJsonString(objJson, "description");

        if (!cmd.command.empty()) {
            commands.push_back(cmd);
        }

        pos = objEnd + 1;

        // Check if we reached array end
        size_t nextComma = resultJson.find(',', pos);
        size_t arrayEnd = resultJson.find(']', pos);
        if (arrayEnd != std::string::npos && (nextComma == std::string::npos || arrayEnd < nextComma)) {
            break;
        }
    }

    return true;
}

bool TelegramApi::getMyDescription(std::string& description) {
    std::string url = buildUrl("getMyDescription");
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    size_t resultPos = response.body.find("\"result\":");
    if (resultPos == std::string::npos) {
        return true;
    }

    std::string resultJson = response.body.substr(resultPos);
    description = extractJsonString(resultJson, "description");

    return true;
}

bool TelegramApi::getMyName(std::string& name) {
    std::string url = buildUrl("getMyName");
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    size_t resultPos = response.body.find("\"result\":");
    if (resultPos == std::string::npos) {
        return true;
    }

    std::string resultJson = response.body.substr(resultPos);
    name = extractJsonString(resultJson, "name");

    return true;
}

bool TelegramApi::getMyShortDescription(std::string& shortDesc) {
    std::string url = buildUrl("getMyShortDescription");
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    size_t resultPos = response.body.find("\"result\":");
    if (resultPos == std::string::npos) {
        return true;
    }

    std::string resultJson = response.body.substr(resultPos);
    shortDesc = extractJsonString(resultJson, "short_description");

    return true;
}

bool TelegramApi::getUpdates(std::vector<Update>& updates, int limit) {
    std::ostringstream urlStream;
    urlStream << buildUrl("getUpdates");
    urlStream << "?limit=" << limit << "&timeout=0";

    std::string url = urlStream.str();
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    // Extract updates from result array
    size_t resultPos = response.body.find("\"result\":");
    if (resultPos == std::string::npos) {
        return true; // Empty updates
    }

    std::string resultJson = response.body.substr(resultPos);

    size_t arrayStart = resultJson.find('[');
    if (arrayStart == std::string::npos) {
        return true; // Empty array
    }

    // Parse updates
    size_t pos = arrayStart + 1;
    while (pos < resultJson.length()) {
        size_t objStart = resultJson.find('{', pos);
        if (objStart == std::string::npos) break;

        // Find matching closing brace
        int braceCount = 1;
        size_t objEnd = objStart + 1;
        while (objEnd < resultJson.length() && braceCount > 0) {
            if (resultJson[objEnd] == '{') braceCount++;
            else if (resultJson[objEnd] == '}') braceCount--;
            objEnd++;
        }

        if (braceCount != 0) break;

        std::string objJson = resultJson.substr(objStart, objEnd - objStart);

        Update update;
        update.updateId = extractJsonInt(objJson, "update_id");

        // Determine update type and extract relevant fields
        if (objJson.find("\"message\":") != std::string::npos) {
            update.type = "message";
            size_t msgPos = objJson.find("\"message\":");
            std::string msgJson = objJson.substr(msgPos);

            // Extract message fields
            update.timestamp = extractJsonInt(msgJson, "date");
            update.text = extractJsonString(msgJson, "text");

            // Extract from user
            size_t fromPos = msgJson.find("\"from\":");
            if (fromPos != std::string::npos) {
                std::string fromJson = msgJson.substr(fromPos);
                update.userId = extractJsonInt(fromJson, "id");
                update.username = extractJsonString(fromJson, "username");
                update.firstName = extractJsonString(fromJson, "first_name");
            }

            // Extract chat info
            size_t chatPos = msgJson.find("\"chat\":");
            if (chatPos != std::string::npos) {
                std::string chatJson = msgJson.substr(chatPos);
                update.chatId = extractJsonInt(chatJson, "id");
                update.chatType = extractJsonString(chatJson, "type");
                update.chatTitle = extractJsonString(chatJson, "title");
            }
        } else if (objJson.find("\"edited_message\":") != std::string::npos) {
            update.type = "edited_message";
            size_t msgPos = objJson.find("\"edited_message\":");
            std::string msgJson = objJson.substr(msgPos);

            update.timestamp = extractJsonInt(msgJson, "date");
            update.text = extractJsonString(msgJson, "text");

            size_t fromPos = msgJson.find("\"from\":");
            if (fromPos != std::string::npos) {
                std::string fromJson = msgJson.substr(fromPos);
                update.userId = extractJsonInt(fromJson, "id");
                update.username = extractJsonString(fromJson, "username");
                update.firstName = extractJsonString(fromJson, "first_name");
            }

            size_t chatPos = msgJson.find("\"chat\":");
            if (chatPos != std::string::npos) {
                std::string chatJson = msgJson.substr(chatPos);
                update.chatId = extractJsonInt(chatJson, "id");
                update.chatType = extractJsonString(chatJson, "type");
                update.chatTitle = extractJsonString(chatJson, "title");
            }
        } else if (objJson.find("\"channel_post\":") != std::string::npos) {
            update.type = "channel_post";
            size_t msgPos = objJson.find("\"channel_post\":");
            std::string msgJson = objJson.substr(msgPos);

            update.timestamp = extractJsonInt(msgJson, "date");
            update.text = extractJsonString(msgJson, "text");

            size_t chatPos = msgJson.find("\"chat\":");
            if (chatPos != std::string::npos) {
                std::string chatJson = msgJson.substr(chatPos);
                update.chatId = extractJsonInt(chatJson, "id");
                update.chatType = extractJsonString(chatJson, "type");
                update.chatTitle = extractJsonString(chatJson, "title");
            }
        } else if (objJson.find("\"callback_query\":") != std::string::npos) {
            update.type = "callback_query";
        } else {
            update.type = "other";
        }

        updates.push_back(update);

        pos = objEnd;

        size_t nextComma = resultJson.find(',', pos);
        size_t arrayEnd = resultJson.find(']', pos);
        if (arrayEnd != std::string::npos && (nextComma == std::string::npos || arrayEnd < nextComma)) {
            break;
        }
    }

    return true;
}

bool TelegramApi::getChat(long long chatId, ChatInfo& chatInfo) {
    std::ostringstream urlStream;
    urlStream << buildUrl("getChat");
    urlStream << "?chat_id=" << chatId;

    std::string url = urlStream.str();
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    size_t resultPos = response.body.find("\"result\":");
    if (resultPos == std::string::npos) {
        return false;
    }

    std::string resultJson = response.body.substr(resultPos);

    chatInfo.chatId = extractJsonInt(resultJson, "id");
    chatInfo.type = extractJsonString(resultJson, "type");
    chatInfo.title = extractJsonString(resultJson, "title");
    chatInfo.username = extractJsonString(resultJson, "username");
    chatInfo.description = extractJsonString(resultJson, "description");
    chatInfo.hasProtectedContent = extractJsonBool(resultJson, "has_protected_content");

    return true;
}

bool TelegramApi::getChatAdministrators(long long chatId, std::vector<ChatAdmin>& admins) {
    std::ostringstream urlStream;
    urlStream << buildUrl("getChatAdministrators");
    urlStream << "?chat_id=" << chatId;

    std::string url = urlStream.str();
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    size_t resultPos = response.body.find("\"result\":");
    if (resultPos == std::string::npos) {
        return true; // Empty admins list
    }

    std::string resultJson = response.body.substr(resultPos);

    size_t arrayStart = resultJson.find('[');
    if (arrayStart == std::string::npos) {
        return true; // Empty array
    }

    // Parse admins
    size_t pos = arrayStart + 1;
    while (pos < resultJson.length()) {
        size_t objStart = resultJson.find('{', pos);
        if (objStart == std::string::npos) break;

        int braceCount = 1;
        size_t objEnd = objStart + 1;
        while (objEnd < resultJson.length() && braceCount > 0) {
            if (resultJson[objEnd] == '{') braceCount++;
            else if (resultJson[objEnd] == '}') braceCount--;
            objEnd++;
        }

        if (braceCount != 0) break;

        std::string objJson = resultJson.substr(objStart, objEnd - objStart);

        ChatAdmin admin;
        admin.status = extractJsonString(objJson, "status");

        // Extract user info
        size_t userPos = objJson.find("\"user\":");
        if (userPos != std::string::npos) {
            std::string userJson = objJson.substr(userPos);
            admin.userId = extractJsonInt(userJson, "id");
            admin.username = extractJsonString(userJson, "username");
            admin.firstName = extractJsonString(userJson, "first_name");
        }

        admin.isAnonymous = extractJsonBool(objJson, "is_anonymous");
        admin.customTitle = extractJsonString(objJson, "custom_title");

        if (admin.userId != 0) {
            admins.push_back(admin);
        }

        pos = objEnd;

        size_t nextComma = resultJson.find(',', pos);
        size_t arrayEnd = resultJson.find(']', pos);
        if (arrayEnd != std::string::npos && (nextComma == std::string::npos || arrayEnd < nextComma)) {
            break;
        }
    }

    return true;
}

bool TelegramApi::getChatMemberCount(long long chatId, int& count) {
    std::ostringstream urlStream;
    urlStream << buildUrl("getChatMemberCount");
    urlStream << "?chat_id=" << chatId;

    std::string url = urlStream.str();
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    // Result is directly an integer
    size_t resultPos = response.body.find("\"result\":");
    if (resultPos == std::string::npos) {
        return false;
    }

    std::string resultJson = response.body.substr(resultPos);
    count = static_cast<int>(extractJsonInt(resultJson, "result"));

    return true;
}

bool TelegramApi::sendMessage(long long chatId, const std::string& text, long long& messageId,
                              const std::string& parseMode, bool disableNotification,
                              bool disableWebPagePreview) {
    std::ostringstream urlStream;
    urlStream << buildUrl("sendMessage");
    urlStream << "?chat_id=" << chatId;

    // URL encode the text (basic implementation)
    std::string encodedText;
    for (char c : text) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encodedText += c;
        } else if (c == ' ') {
            encodedText += '+';
        } else {
            char hex[4];
            snprintf(hex, sizeof(hex), "%%%02X", static_cast<unsigned char>(c));
            encodedText += hex;
        }
    }

    urlStream << "&text=" << encodedText;

    // Add parse_mode if specified
    if (!parseMode.empty()) {
        urlStream << "&parse_mode=" << parseMode;
    }

    // Add disable_notification if true
    if (disableNotification) {
        urlStream << "&disable_notification=true";
    }

    // Add disable_web_page_preview if true
    if (disableWebPagePreview) {
        urlStream << "&disable_web_page_preview=true";
    }

    std::string url = urlStream.str();
    HttpResponse response = httpClient_->get(url);

    if (!response.success) {
        lastResponse_.ok = false;
        lastResponse_.description = "HTTP request failed: " + response.error;
        return false;
    }

    parseJsonResponse(response.body, lastResponse_);

    if (!lastResponse_.ok) {
        return false;
    }

    // Extract message_id from result
    size_t resultPos = response.body.find("\"result\":");
    if (resultPos != std::string::npos) {
        std::string resultJson = response.body.substr(resultPos);
        messageId = extractJsonInt(resultJson, "message_id");
    }

    return true;
}

} // namespace TelegramDigger
