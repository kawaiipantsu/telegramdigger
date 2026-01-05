#include "http_client.h"
#include <curl/curl.h>
#include <cstring>

namespace TelegramDigger {

// Callback function for curl to write data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), realsize);
    return realsize;
}

// PIMPL implementation
struct HttpClient::Impl {
    CURL* curl;
    struct curl_slist* headers;
    long timeout;
    bool sslVerify;
    std::map<std::string, std::string> customHeaders;

    Impl() : curl(nullptr), headers(nullptr), timeout(30), sslVerify(true) {}

    ~Impl() {
        if (headers) {
            curl_slist_free_all(headers);
        }
        if (curl) {
            curl_easy_cleanup(curl);
        }
    }
};

HttpClient::HttpClient() : impl_(std::make_unique<Impl>()) {
    init();
}

HttpClient::~HttpClient() {
    cleanup();
}

void HttpClient::init() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    impl_->curl = curl_easy_init();
}

void HttpClient::cleanup() {
    curl_global_cleanup();
}

void HttpClient::setHeader(const std::string& key, const std::string& value) {
    impl_->customHeaders[key] = value;
}

void HttpClient::clearHeaders() {
    impl_->customHeaders.clear();
    if (impl_->headers) {
        curl_slist_free_all(impl_->headers);
        impl_->headers = nullptr;
    }
}

void HttpClient::setTimeout(long seconds) {
    impl_->timeout = seconds;
}

void HttpClient::setSSLVerification(bool verify) {
    impl_->sslVerify = verify;
}

HttpResponse HttpClient::get(const std::string& url) {
    HttpResponse response;

    if (!impl_->curl) {
        response.error = "CURL not initialized";
        return response;
    }

    std::string responseBody;

    // Set URL
    curl_easy_setopt(impl_->curl, CURLOPT_URL, url.c_str());

    // Set callback function
    curl_easy_setopt(impl_->curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(impl_->curl, CURLOPT_WRITEDATA, &responseBody);

    // Set timeout
    curl_easy_setopt(impl_->curl, CURLOPT_TIMEOUT, impl_->timeout);

    // Set SSL verification
    curl_easy_setopt(impl_->curl, CURLOPT_SSL_VERIFYPEER, impl_->sslVerify ? 1L : 0L);
    curl_easy_setopt(impl_->curl, CURLOPT_SSL_VERIFYHOST, impl_->sslVerify ? 2L : 0L);

    // Set custom headers
    struct curl_slist* headers = nullptr;
    for (const auto& header : impl_->customHeaders) {
        std::string headerStr = header.first + ": " + header.second;
        headers = curl_slist_append(headers, headerStr.c_str());
    }
    if (headers) {
        curl_easy_setopt(impl_->curl, CURLOPT_HTTPHEADER, headers);
    }

    // Perform request
    CURLcode res = curl_easy_perform(impl_->curl);

    // Free headers
    if (headers) {
        curl_slist_free_all(headers);
    }

    if (res != CURLE_OK) {
        response.error = curl_easy_strerror(res);
        response.success = false;
        return response;
    }

    // Get response code
    long responseCode;
    curl_easy_getinfo(impl_->curl, CURLINFO_RESPONSE_CODE, &responseCode);

    response.statusCode = static_cast<int>(responseCode);
    response.body = responseBody;
    response.success = (responseCode >= 200 && responseCode < 300);

    return response;
}

HttpResponse HttpClient::post(const std::string& url, const std::string& data) {
    HttpResponse response;

    if (!impl_->curl) {
        response.error = "CURL not initialized";
        return response;
    }

    std::string responseBody;

    // Set URL
    curl_easy_setopt(impl_->curl, CURLOPT_URL, url.c_str());

    // Set POST
    curl_easy_setopt(impl_->curl, CURLOPT_POST, 1L);

    // Set POST data
    if (!data.empty()) {
        curl_easy_setopt(impl_->curl, CURLOPT_POSTFIELDS, data.c_str());
    }

    // Set callback function
    curl_easy_setopt(impl_->curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(impl_->curl, CURLOPT_WRITEDATA, &responseBody);

    // Set timeout
    curl_easy_setopt(impl_->curl, CURLOPT_TIMEOUT, impl_->timeout);

    // Set SSL verification
    curl_easy_setopt(impl_->curl, CURLOPT_SSL_VERIFYPEER, impl_->sslVerify ? 1L : 0L);
    curl_easy_setopt(impl_->curl, CURLOPT_SSL_VERIFYHOST, impl_->sslVerify ? 2L : 0L);

    // Set custom headers
    struct curl_slist* headers = nullptr;
    for (const auto& header : impl_->customHeaders) {
        std::string headerStr = header.first + ": " + header.second;
        headers = curl_slist_append(headers, headerStr.c_str());
    }
    if (headers) {
        curl_easy_setopt(impl_->curl, CURLOPT_HTTPHEADER, headers);
    }

    // Perform request
    CURLcode res = curl_easy_perform(impl_->curl);

    // Free headers
    if (headers) {
        curl_slist_free_all(headers);
    }

    if (res != CURLE_OK) {
        response.error = curl_easy_strerror(res);
        response.success = false;
        return response;
    }

    // Get response code
    long responseCode;
    curl_easy_getinfo(impl_->curl, CURLINFO_RESPONSE_CODE, &responseCode);

    response.statusCode = static_cast<int>(responseCode);
    response.body = responseBody;
    response.success = (responseCode >= 200 && responseCode < 300);

    return response;
}

} // namespace TelegramDigger
