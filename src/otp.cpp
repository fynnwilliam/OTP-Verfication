#include "otp.h"
#include "smtp.h"

#include <algorithm>
#include <iterator>
#include <exception>
#include <random>
#include <chrono>
#include <cctype>
#include <utility>
#include <regex>

otp::otp(std::string s, std::string p, std::string m)
        : sender_{std::move(s)}, password_{std::move(p)}, mailserver_{std::move(m)} {}

void otp::generate_code_r() noexcept
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::uniform_int_distribution<int> distrib(9'999, 99'999);

    code_ = std::to_string(distrib(g));
}

void otp::inquire() const noexcept
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

void otp::to_lower() noexcept
{
    std::transform(recipient_.begin(), recipient_.end(), recipient_.begin(),
        [](unsigned char c)
        { return std::tolower(c); });
}

void otp::trim_email() noexcept
{
    remove_leading_spaces();
    remove_trailing_spaces();
}

bool otp::is_recipient_valid() const noexcept
{
    std::regex pattern{"(\\w+)(\\.|_)?(\\w+)@(\\w+)(\\.(\\w+))+"};
    return std::regex_match(recipient_, pattern);
}

std::vector<char> otp::generate_characters() const noexcept
{
    std::vector<char> characters;
    for (int i = 48; i < 123; ++i)
        if (std::isalpha(i) || std::isdigit(i))
            characters.push_back(i);
    return characters;
}

int otp::retry() const noexcept
{
    std::cout << (recipient_ == smtp{}.sender() ? "\tno please, 😃" : "\tcheck for typos or extra spaces and") << " try again: ";
    return -1;
}

int otp::verify_recipient() const noexcept
{
    return recipient_ == smtp{}.sender() ? retry() : is_recipient_valid() ? 0
                                                                  : retry();
}

void otp::remove_leading_spaces() noexcept
{
    while (recipient_.starts_with(' '))
        recipient_.erase(0, 1);
}

void otp::remove_trailing_spaces() noexcept
{
    while (recipient_.ends_with(' '))
        recipient_.pop_back();
}

void otp::authenticate_email_s()
{
    generate_code_s();
    certify();
}

void otp::authenticate_email_r()
{
    generate_code_r();
    certify();
}

void otp::generate_code_s() noexcept
{
    auto&& v = generate_characters();
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});

    code_.clear();
    std::copy(v.begin(), v.begin() + 5, std::back_inserter(code_));
}

void otp::display_info() const noexcept
{
    std::cout << "\nA temporary code has been sent to your email address.\n"
              << "If you do not receive this message in the next few minutes,\n"
              << "please check your junk mail folder or check for typos.\n"
              << "\n\tcode: ";
}

auto otp::submit_code() const
{
    smtp _smtp(recipient_, code_);
    _smtp.send_email() ? throw std::runtime_error("...") : display_info();

    return std::chrono::system_clock::now();
}

void otp::certify() const
{
    auto start = submit_code();
    verify_code(start);
}

int otp::declare(auto const& start) const noexcept
{
    using namespace std::chrono;
    using namespace std::chrono_literals;
    duration<int> elapsed_time = duration_cast<minutes>(system_clock::now() - start);

    std::cout << (elapsed_time <= 5min ? "\ngreat! you got it...\n\n" : "code has expired, kindly make a new request.\n");
    return 0;
}

template <typename T>
int otp::verify_input(T const& start, std::string const& input) const noexcept
{
    return input == code_ ? declare(start) : retry();
}

template <typename T>
void otp::verify_code(T const& start) const noexcept
{
    std::string input;
    while (std::getline(std::cin, input))
        if (!verify_input(start, input))
            break;
}