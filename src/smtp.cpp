#include "smtp.h"

std::vector<std::string> smtp::payload_text_{
    "to: ", "place_holder_recipient", "\n",
    "from: ", "place_holder_sender", "\n",
    "subject: OTP_SMTP_TEST\n",
    "Thank you for trying this out...\n\n",
    "\t\t\t", "place_holder_code", "\n\n",
    "Enter the code when asked for it to show that you got this email.\n\n",
    "If you don't recognize the subject, someone probably gave your email address by mistake.\n",
    "You can safely ignore this email.\n\n",
    "Sincerely,\n",
    "sender",
    "\0"
};

std::string const& smtp::sender() const noexcept
{
    return sender_;
}

size_t smtp::copy_data_to_ptr(char* ptr, std::string const& data, upload_status* upload_ctx)
{
    std::memcpy(ptr, data.c_str(), data.size());
    upload_ctx->lines_read++;

    return data.size();
}

size_t smtp::payload_source(char* ptr, size_t size, size_t nmemb, void* userp)
{
    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1))
        return 0;

    upload_status* upload_ctx = (upload_status*)userp;
    std::string data = payload_text_[upload_ctx->lines_read];

    return !data.empty() ? copy_data_to_ptr(ptr, data, upload_ctx) : 0;
}

void smtp::establish_connection()
{
    set_credentials();
    connect_to_mailserver();
    specify_sender();
    enable_debug_info(0L);
}

void smtp::specify_sender()
{
    curl_easy_setopt(curl_, CURLOPT_MAIL_FROM, sender_.c_str());
}

void smtp::set_credentials()
{
    curl_easy_setopt(curl_, CURLOPT_USERNAME, sender_.c_str());
    curl_easy_setopt(curl_, CURLOPT_PASSWORD, password_.c_str());
}

void smtp::connect_to_mailserver()
{
    curl_easy_setopt(curl_, CURLOPT_URL, mailserver_.c_str());
    curl_easy_setopt(curl_, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
}

CURLcode smtp::send_message()
{
    return curl_easy_perform(curl_);
}

int smtp::check_status(CURLcode c_status)
{
    if (c_status != CURLE_OK)
        std::cerr << "email not sent: " << curl_easy_strerror(c_status) << '\n';
    return (int)c_status;
}

void smtp::cleanup()
{
    curl_slist_free_all(recipients_);
    curl_easy_cleanup(curl_);
}

void smtp::enable_debug_info(long choice)
{
    curl_easy_setopt(curl_, CURLOPT_VERBOSE, choice);
}

int smtp::send_email()
{
    if (curl_)
    {
        establish_connection();
        add_recipients();
        read_data();

        status_ = check_status(send_message());
    }
    return status_;
}

void smtp::update_code()
{
    payload_text_[9] = code_;
}

void smtp::add_recipients()
{
    recipients_ = curl_slist_append(recipients_, recipient_.c_str());
    curl_easy_setopt(curl_, CURLOPT_MAIL_RCPT, recipients_);
}

void smtp::update_recipient()
{
    payload_text_[1] = recipient_;
}

void smtp::update_sender()
{
    payload_text_[4] = sender_;
}

void smtp::read_data()
{
    update_payload_text();

    curl_easy_setopt(curl_, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl_, CURLOPT_READDATA, &upload_ctx_);
    curl_easy_setopt(curl_, CURLOPT_UPLOAD, 1L);
}

void smtp::update_payload_text()
{
    update_recipient();
    update_sender();
    update_code();
}