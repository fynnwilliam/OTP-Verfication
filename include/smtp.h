#pragma once
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <curl/curl.h>

class smtp
{
private:
    struct upload_status
    {
        int lines_read{};
    };

    curl_slist* recipients_{nullptr};
    CURL* curl_;
    int status_{-1};
    upload_status upload_ctx_;
    std::string const sender_;
    std::string const password_;
    std::string const mailserver_;
    std::string const recipient_;
    std::string const code_;

    static std::vector<std::string> payload_text_;

    static size_t copy_data_to_ptr(char* ptr, std::string const& data, upload_status* upload_ctx);
    static size_t payload_source(char* ptr, size_t size, size_t nmemb, void* userp);
    void establish_connection();
    void set_credentials();
    void connect_to_mailserver();
    CURLcode send_message();
    int check_status(CURLcode status);
    void cleanup();
    void update_code();
    void add_recipients();
    void update_recipient();
    void read_data();
    void update_payload_text();
    void enable_debug_info(long choice);
    void specify_sender();
    void update_sender();
    
public:
    smtp(std::string const& s, std::string const& p, std::string const& m, std::string const& r, std::string const& c)
        : curl_{curl_easy_init()}, sender_{s}, password_{p}, mailserver_{m}, recipient_{r}, code_{c} {}

    int send_email();
    ~smtp() { cleanup(); }
};