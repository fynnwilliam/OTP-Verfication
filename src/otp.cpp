#include "otp.h"
#include "smtp.h"

void otp::generate_code_r()
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::uniform_int_distribution<int> distrib(9'999, 99'999);

    code_ = std::to_string(distrib(g));
}

void otp::inquire() const
{
    std::cout << "\nkindly enter your email address for the test...\n"
              << "\n\temail: ";
}

void otp::recipient_email()
{
    inquire();

    while (std::getline(std::cin, recipient_))
    {
        trim_email();
        to_lower();
        if (!verify_recipient())
            break;
    }
}

void otp::to_lower()
{
    std::transform(recipient_.begin(), recipient_.end(), recipient_.begin(),
        [](unsigned char c)
        { return std::tolower(c); });
}

void otp::trim_email()
{
    remove_leading_spaces();
    remove_trailing_spaces();
}

bool otp::is_recipient_valid() const
{
    return std::regex_match(recipient_, pattern);
}

void otp::generate_characters()
{
    for (int i = 48; i < 123; i++)
        if (std::isalpha(i) || std::isdigit(i))
            v_.push_back(i);
}

int otp::retry() const
{
    std::cout << (recipient_ == sender_ ? "\tno please, 😃" : "\tcheck for typos or extra spaces and") << " try again: ";
    return -1;
}

int otp::verify_recipient() const
{
    return recipient_ == sender_ ? retry() : is_recipient_valid() ? 0
                                                                  : retry();
}

void otp::remove_leading_spaces()
{
    while (recipient_.starts_with(' '))
        recipient_.erase(0, 1);
}

void otp::remove_trailing_spaces()
{
    while (recipient_.ends_with(' '))
        recipient_.pop_back();
}

void otp::authenticate_email_s()
{

    generate_characters();
    generate_code_s();

    certify();
}

void otp::authenticate_email_r()
{
    generate_code_r();
    certify();
}

void otp::generate_code_s()
{
    std::shuffle(v_.begin(), v_.end(), std::mt19937{std::random_device{}()});

    code_.clear();
    std::copy(v_.begin(), v_.begin() + 5, std::back_inserter(code_));
}

void otp::display_info() const
{
    std::cout << "\nA temporary code has been sent to your email address.\n"
              << "If you do not receive this message in the next few minutes,\n"
              << "please check your junk mail folder or check for typos.\n"
              << "\n\tcode: ";
}

auto otp::submit_code()
{
    smtp smtp_test(sender_, password_, mailserver_, recipient_, code_);
    status_ = smtp_test.send_email();
    status_ != 0 ? throw std::runtime_error("...") : display_info();

    return std::chrono::system_clock::now();
}

void otp::certify()
{
    auto start_ = submit_code();
    verify_code(start_);
}

int otp::declare(auto const& start)
{
    using namespace std::chrono;
    using namespace std::chrono_literals;
    duration<int> elapsed_time = duration_cast<minutes>(system_clock::now() - start);

    std::cout << (elapsed_time <= 5min ? "\ngreat! you got it...\n\n" : "code has expired, kindly make a new request.\n");
    return 0;
}

template <typename T>
int otp::verify_input(T const& start)
{
    return input_ == code_ ? declare(start) : retry();
}

template <typename T>
void otp::verify_code(T const& start)
{
    while (std::getline(std::cin, input_))
        if (!verify_input(start))
            break;
}