#include "smtp.h"

std::string const sender{"fynnwilliam@gmail.com"};
curl_slist *recipients = nullptr;
int status{-1};
CURL *curl = curl_easy_init();

static std::vector<std::string> payload_text{
    "to: ", "place_holder_recipient", "\n",
    "from: ", sender.c_str(), "\n",
    "subject: OTP_SMTP_TEST\n",
    "Thank you for trying this out...\n\n",
    "\t\t\t", "place_holder_code", "\n\n",
    "Enter the code when asked for it to show that you got this email.\n\n",
    "If you don't recognize the subject, someone probably gave your email address by mistake.\n",
    "You can safely ignore this email.\n\n",
    "Sincerely,\n",
    "William",
    "\0"
    };

size_t copy_data_to_ptr(char *ptr, std::string const &data, upload_status *upload_ctx)
{
    std::memcpy(ptr, data.c_str(), data.size());
    upload_ctx->lines_read++;

    return data.size();
}

static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1))
        return 0;

    upload_status *upload_ctx = (upload_status *)userp;
    std::string data = payload_text[upload_ctx->lines_read];

    return !data.empty() ? copy_data_to_ptr(ptr, data, upload_ctx) : 0;
}

void establish_connection()
{
    set_credentials();
    connect_to_mailserver();
    specify_sender();
    enable_debug_info(0L);
}

void specify_sender()
{
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, sender.c_str());
}

void set_credentials()
{
    curl_easy_setopt(curl, CURLOPT_USERNAME, sender.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "password");
}

void connect_to_mailserver()
{
    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
}

CURLcode send_message()
{
    return curl_easy_perform(curl);
}

int check_status(CURLcode status)
{
    if (status != CURLE_OK)
        std::cout << "email not sent: " << curl_easy_strerror(status) << '\n';
    return (int)status;
}

void cleanup()
{
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
}

void enable_debug_info(long choice)
{
    curl_easy_setopt(curl, CURLOPT_VERBOSE, choice);
}

int send_email(/*CURL *curl, */std::string const &recipient, std::string const &code)
{
    if(curl)
    {
        establish_connection();

        add_recipients(recipient);

        read_data(curl, upload_status(), recipient, code);

        status = check_status(send_message());

        cleanup();
    }
    return status;
}

void update_code(std::string const &code)
{
    payload_text[9] = code;
}

void add_recipients(std::string const &recipient)
{
    recipients = curl_slist_append(recipients, recipient.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
}

void update_recipient(std::string const &recipient)
{
    payload_text[1] = recipient;
}

void read_data(CURL *curl, upload_status upload_ctx, std::string const &recipient, std::string const &code)
{
    update_payload_text(recipient, code);
    
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
}

void update_payload_text(std::string const &recipient, std::string const &code)
{
    update_recipient(recipient);
    update_code(code);
}