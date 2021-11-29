#include "otp.h"
#include "smtp.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <exception>
#include <random>
#include <chrono>
#include <cctype>
#include <utility>
#include <regex>

void otp::generate_code_r() noexcept
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::uniform_int_distribution<int> distrib(9'999, 99'999);

    code() = std::to_string(distrib(g));
}

void otp::inquire() const noexcept
{
    std::cout << "\nkindly enter your email address for the test...\n"
              << "\n\temail: ";
}

void otp::recipient_email()
{
    inquire();
    while (std::getline(std::cin, recipient()))
    {
        trim_email();
        to_lower();
        if (!verify_recipient())
            break;
    }
}

void otp::to_lower() noexcept
{
    std::transform(recipient().begin(), recipient().end(), recipient().begin(),
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
    return std::regex_match(recipient(), pattern);
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
    std::cout << "\tcheck for typos or extra spaces and try again: ";
    return -1;
}

int otp::invalid_recipient() const noexcept
{
    std::cout << "\tthe email address is already in use, please use another address: ";
    return -1;
}

int otp::verify_recipient() const noexcept
{
    return recipient() == smtp{}.sender() ? invalid_recipient() :
                     is_recipient_valid() ? 0                   : retry();
}

void otp::remove_leading_spaces() noexcept
{
    while (recipient().starts_with(' '))
        recipient().erase(0, 1);
}

void otp::remove_trailing_spaces() noexcept
{
    while (recipient().ends_with(' '))
        recipient().pop_back();
}

void otp::authenticate_email_s()
{
    recipient_email();
    generate_code_s();
    certify();
}

void otp::authenticate_email_r()
{
    recipient_email();
    generate_code_r();
    certify();
}

void otp::generate_code_s() noexcept
{
    auto&& v = generate_characters();
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});

    code().clear();
    std::copy(v.begin(), v.begin() + 5, std::back_inserter(code()));
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
    smtp _smtp(recipient(), code());
    _smtp.send_email() ? throw std::runtime_error("...") : display_info();

    return std::chrono::system_clock::now();
}

void otp::certify() const
{
    auto start = submit_code();
    verify_code(start);
}

int otp::success() const noexcept
{
    std::cout << "\ngreat! you got it...\n\n";
    return 0;
}

int otp::make_new_request() const noexcept
{
    std::cout << "code has expired, kindly make a new request.\n";
    return 0;
}

int otp::declare(auto const& start) const noexcept
{
    using namespace std::chrono;
    using namespace std::chrono_literals;
    duration<int> elapsed_time = duration_cast<minutes>(system_clock::now() - start);

    return elapsed_time <= 5min ? success() : make_new_request();
}

std::string const& otp::recipient() const noexcept
{
    return recipient_;
}

std::string& otp::recipient() noexcept
{
    return recipient_;
}

std::string& otp::code() noexcept
{
    return code_;
}

std::string const& otp::code() const noexcept
{
    return code_;
}

template <typename T>
int otp::verify_input(T const& start, std::string const& input) const noexcept
{
    return input == code() ? declare(start) : retry();
}

template <typename T>
void otp::verify_code(T const& start) const noexcept
{
    std::string input;
    while (std::getline(std::cin, input))
        if (!verify_input(start, input))
            break;
}