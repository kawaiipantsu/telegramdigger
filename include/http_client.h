#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <map>
#include <memory>

namespace TelegramDigger {

/**
 * HTTP response structure
 */
struct HttpResponse {
    int statusCode;
    std::string body;
    std::map<std::string, std::string> headers;
    bool success;
    std::string error;

    HttpResponse() : statusCode(0), success(false) {}
};

/**
 * Simple HTTP client using libcurl
 */
class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    /**
     * Perform GET request
     */
    HttpResponse get(const std::string& url);

    /**
     * Perform POST request
     */
    HttpResponse post(const std::string& url, const std::string& data = "");

    /**
     * Set custom header
     */
    void setHeader(const std::string& key, const std::string& value);

    /**
     * Clear all custom headers
     */
    void clearHeaders();

    /**
     * Set connection timeout in seconds
     */
    void setTimeout(long seconds);

    /**
     * Enable/disable SSL verification
     */
    void setSSLVerification(bool verify);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    /**
     * Initialize curl
     */
    void init();

    /**
     * Cleanup curl
     */
    void cleanup();
};

} // namespace TelegramDigger

#endif // HTTP_CLIENT_H
