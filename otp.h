#include <algorithm>
#include <iterator>
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <cctype>
#include <regex>

class otp
{
private:
    std::string email_{};
    std::vector<char> v_{};
    std::string code_{};
    const std::regex pattern;
    int status_{};
    std::string input_{};
    std::string const sender_{"fynnwilliam@gmail.com"};

public:
    otp() : input_{-1}, pattern{"(\\w+)(\\.|_)?(\\w+)@(\\w+)(\\.(\\w+))+"} {}
    void generate_code_r();
    void inquire();
    void recipient_email();
    void to_lower();
    void trim_email();
    bool is_email_valid();
    void generate_characters();
    int retry();
    int verify_email();
    void remove_leading_spaces();
    void remove_trailing_spaces();
    void authenticate_email_s();
    void authenticate_email_r();
    void generate_code_s();
    void display_info();
    auto submit_code();
    void certify();
    int declare(auto const &start);

    template <typename T>
    int verify_input(T const &start);

    template <typename T>
    void verify_code(T const &start);
};

// https://www.ascii-code.com/
// https://en.cppreference.com/w/cpp/algorithm/copy
// https://www.merriam-webster.com/thesaurus/observe
// https://en.cppreference.com/w/cpp/error/exception
// https://www.merriam-webster.com/dictionary/certify
// https://en.cppreference.com/w/cpp/string/byte/isalpha
// https://en.cppreference.com/w/cpp/algorithm/random_shuffle
// https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution