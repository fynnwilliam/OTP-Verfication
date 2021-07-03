#pragma once
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <curl/curl.h>

struct upload_status
{
    int lines_read{};
};


size_t copy_data_to_ptr(char *ptr, char const *data, upload_status *upload_ctx);
static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp);
void establish_connection();
void set_credentials();
void connect_to_mailserver();
CURLcode send_message();
int check_status(CURLcode status);
void cleanup();
int send_email(std::string const &recipient, std::string const &code);
void update_code(std::string const &code);
void add_recipients(std::string const &recipient);
void update_recipient(std::string const &recipient);
void read_data(CURL *curl, upload_status upload_ctx, std::string const &recipient, std::string const &code);
void update_payload_text(std::string const &recipient, std::string const &code);
void enable_debug_info(long choice);
void specify_sender();


// http://www.cplusplus.com/forum/general/18964/
// https://en.cppreference.com/w/cpp/string/byte/memcpy
// https://en.cppreference.com/w/cpp/algorithm/transform